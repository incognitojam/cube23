#!/bin/bash
set -e

# Test script for cube23 CI Docker environment
# 
# Usage:
#   Local development: ./test.sh
#     - Auto-detects platform: X11 forwarding on Mac (XQuartz) and Linux
#     - Shows actual Vulkan window from Docker-built binary
#   
#   Force headless mode: ./test.sh --headless
#     - Always uses Docker with xvfb
#   
#   Clean build: ./test.sh --clean
#     - Removes build directory before building
#   
#   CI environment: ./test.sh
#     - Uses pre-built image from workflow, runs tests only
#     - Automatically detects GitHub Actions environment
#
# Requirements:
#   - macOS: XQuartz (brew install --cask xquartz)
#   - Linux: X11 server running

# Docker image configuration
IMAGE_NAME="cube23-ci"
IMAGE_TAG="latest"
DOCKERFILE="Dockerfile.ci"

# Parse command line flags
HEADLESS=false
CLEAN=false
for arg in "$@"; do
    case $arg in
        --headless)
            HEADLESS=true
            ;;
        --clean)
            CLEAN=true
            ;;
        *)
            echo "Unknown option: $arg"
            echo "Usage: $0 [--headless] [--clean]"
            echo "  --headless  Force headless mode"
            echo "  --clean     Remove build directory before building"
            exit 1
            ;;
    esac
done

# Determine workspace path and build command based on environment
if [ "${GITHUB_ACTIONS}" = "true" ]; then
    WORKSPACE_PATH="${GITHUB_WORKSPACE}"
    BUILD_IMAGE=false
    EXECUTION_MODE="ci"
    echo "🔧 Running in GitHub Actions CI environment"
elif [ "$HEADLESS" = "true" ]; then
    WORKSPACE_PATH="$(pwd)"
    BUILD_IMAGE=true
    EXECUTION_MODE="docker_headless"
    echo "🐳 Running in forced headless mode"
else
    # Running locally - detect platform
    WORKSPACE_PATH="$(pwd)"
    BUILD_IMAGE=true
    case "$(uname -s)" in
        Darwin)
            EXECUTION_MODE="mac_x11"
            echo "🍎 Running in local macOS environment (X11 forwarding via XQuartz)"
            ;;
        Linux)
            EXECUTION_MODE="linux_local"
            BUILD_IMAGE=false
            echo "🐧 Running in local Linux environment (native build)"
            ;;
        *)
            EXECUTION_MODE="docker_headless"
            echo "🏠 Running in local development environment (Docker headless)"
            ;;
    esac
fi

# Helper function to run commands in container
run() {
    docker run --rm --platform linux/amd64 \
        -v "${WORKSPACE_PATH}:/workspace" \
        "${IMAGE_NAME}:${IMAGE_TAG}" \
        bash -c "$*"
}

# Build Docker image
if [ "$BUILD_IMAGE" = true ]; then
    echo "🔨 Building Docker image: ${IMAGE_NAME}:${IMAGE_TAG}"
    # Build for amd64 to match GitHub Actions environment
    docker build --platform linux/amd64 -f "$DOCKERFILE" -t "${IMAGE_NAME}:${IMAGE_TAG}" .
    echo "✅ Docker image built successfully"
fi

if [ "$EXECUTION_MODE" != "linux_local" ]; then
    echo "🐳 Running container with workspace: ${WORKSPACE_PATH}"
fi

# Clean build directory if requested
if [ "$CLEAN" = true ]; then
    echo "🧹 Cleaning build directory..."
    if [ "$EXECUTION_MODE" = "linux_local" ]; then
        rm -rf build
    else
        run "rm -rf build"
    fi
    echo "✅ Build directory cleaned"
fi

echo "🚀 Starting build process..."

# Configure CMake
echo "⚙️ Configuring CMake..."
if [ "$EXECUTION_MODE" = "linux_local" ]; then
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
else
    run "cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++"
fi

# Build all targets
echo "🔨 Building all targets..."
if [ "$EXECUTION_MODE" = "linux_local" ]; then
    cmake --build build -j$(nproc)
else
    run "cmake --build build -j\$(nproc)"
fi

# Verify build artifacts
echo "✅ Verifying build artifacts..."
echo "Build directory contents:"
if [ "$EXECUTION_MODE" = "linux_local" ]; then
    ls -la build/
else
    run "ls -la build/"
fi

echo "Checking for executables:"
test -f "${WORKSPACE_PATH}/build/cube23/cube23" && echo "✅ cube23 executable found" || echo "❌ cube23 not found"
test -f "${WORKSPACE_PATH}/build/vkdemo/vkdemo" && echo "✅ vkdemo executable found" || echo "❌ vkdemo not found"
test -f "${WORKSPACE_PATH}/build/vox/libvox.a" && echo "✅ libvox.a library found" || echo "❌ libvox.a not found"

# Verify shader compilation
echo "Checking for shaders:"
if [ ! -d "${WORKSPACE_PATH}/build/vkdemo/shaders" ] || \
   [ ! -f "${WORKSPACE_PATH}/build/vkdemo/shaders/shader.vert.spv" ] || \
   [ ! -f "${WORKSPACE_PATH}/build/vkdemo/shaders/shader.frag.spv" ]; then
    echo "❌ ERROR: vkdemo shaders not compiled!"
    exit 1
fi
echo "✅ vkdemo shaders compiled"

# Test running vkdemo executable
echo "🚀 Testing vkdemo execution..."

case "$EXECUTION_MODE" in
    "linux_local")
        echo "🖥️  Running vkdemo natively with local X11"
        cd build/vkdemo
        TEST_MODE=1 timeout 10s ./vkdemo
        cd - > /dev/null
        ;;
    "mac_x11")
        if ! pgrep -f "XQuartz" > /dev/null; then
            echo "❌ ERROR: XQuartz not running."
            echo "   Start XQuartz and enable: Preferences > Security > ✓ Allow connections from network clients"
            exit 1
        fi

        if ! defaults read org.xquartz.X11 nolisten_tcp 2>/dev/null | grep -q "0"; then
            echo "⚠️  XQuartz network connections may not be enabled. Continuing anyway..."
        fi

        echo "🖥️  Running vkdemo with X11 forwarding via XQuartz"
        
        XQUARTZ_DISPLAY=$(echo $DISPLAY | sed 's/.*://')
        DOCKER_DISPLAY="host.docker.internal:${XQUARTZ_DISPLAY}"

        if command -v xhost >/dev/null 2>&1; then
            xhost +localhost > /dev/null 2>&1
            xhost +127.0.0.1 > /dev/null 2>&1
            xhost +192.168.65.0/24 > /dev/null 2>&1 || true
        fi

        docker run --rm --platform linux/amd64 \
            -v "${WORKSPACE_PATH}:/workspace" \
            -e DISPLAY="${DOCKER_DISPLAY}" \
            -e TEST_MODE=1 \
            --add-host host.docker.internal:host-gateway \
            "${IMAGE_NAME}:${IMAGE_TAG}" \
            bash -c "cd build/vkdemo && timeout 10s ./vkdemo"
        ;;
    "linux_x11")
        echo "🖥️  Running vkdemo with X11 forwarding"
        
        if command -v xhost >/dev/null 2>&1; then
            xhost +local:docker > /dev/null 2>&1
        fi
        
        docker run --rm --platform linux/amd64 \
            -v "${WORKSPACE_PATH}:/workspace" \
            -e DISPLAY="${DISPLAY}" \
            -e TEST_MODE=1 \
            -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
            -v ~/.Xauthority:/root/.Xauthority:ro \
            --network host \
            "${IMAGE_NAME}:${IMAGE_TAG}" \
            bash -c "cd build/vkdemo && timeout 10s ./vkdemo"
        ;;
    "ci"|"docker_headless"|*)
        echo "🤖 Running vkdemo headless with xvfb"
        run "cd build/vkdemo && timeout 10s bash -c 'TEST_MODE=1 xvfb-run -a --server-args=\"-screen 0 800x600x24\" bash -c \"./vkdemo &
        sleep 4
        import -window root /workspace/screenshot_vkdemo.png
        wait\"'"
        ;;
esac

echo "✅ vkdemo executed successfully"

# Test running cube23 executable (OpenGL backend)
echo "🚀 Testing cube23 execution (OpenGL backend)..."

case "$EXECUTION_MODE" in
    "linux_local")
        echo "🖥️  Running cube23 natively with OpenGL"
        cd build/cube23
        TEST_MODE=1 VOX_RENDERER=opengl timeout 10s ./cube23
        cd - > /dev/null
        ;;
    "linux_x11")
        echo "🖥️  Running cube23 with OpenGL and X11 forwarding"

        docker run --rm --platform linux/amd64 \
            -v "${WORKSPACE_PATH}:/workspace" \
            -e DISPLAY="${DISPLAY}" \
            -e TEST_MODE=1 \
            -e VOX_RENDERER=opengl \
            -e LIBGL_ALWAYS_SOFTWARE=1 \
            -e XDG_RUNTIME_DIR=/tmp \
            -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
            --network host \
            "${IMAGE_NAME}:${IMAGE_TAG}" \
            bash -c "cd build/cube23 && timeout 10s ./cube23"
        ;;
    "mac_x11"|"ci"|"docker_headless"|*)
        echo "🤖 Running cube23 headless with OpenGL backend"
        run "cd build/cube23 && timeout 15s bash -c 'TEST_MODE=1 VOX_RENDERER=opengl LIBGL_ALWAYS_SOFTWARE=1 XDG_RUNTIME_DIR=/tmp xvfb-run -a --server-args=\"-screen 0 800x600x24\" bash -c \"./cube23 &
        sleep 4
        import -window root /workspace/screenshot_opengl.png
        wait\"'"
        ;;
esac

echo "✅ cube23 (OpenGL) executed successfully"


echo "🎉 Build and execution tests completed successfully!"
echo "🏁 Test script completed successfully"

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

# Check for --headless flag
HEADLESS=false
if [ "$1" = "--headless" ]; then
    HEADLESS=true
fi

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
            EXECUTION_MODE="linux_x11"
            echo "🐧 Running in local Linux environment (X11 forwarding)"
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

# Build Docker image if running locally
if [ "$BUILD_IMAGE" = true ]; then
    echo "🔨 Building Docker image: ${IMAGE_NAME}:${IMAGE_TAG}"
    # Build for amd64 to match GitHub Actions environment
    docker build --platform linux/amd64 -f "$DOCKERFILE" -t "${IMAGE_NAME}:${IMAGE_TAG}" .
    echo "✅ Docker image built successfully"
fi

echo "🐳 Running container with workspace: ${WORKSPACE_PATH}"
echo "🚀 Starting build process..."

# Configure CMake
echo "⚙️ Configuring CMake..."
run "cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++"

# Build all targets
echo "🔨 Building all targets..."
run "cmake --build build -j\$(nproc)"

# Verify build artifacts
echo "✅ Verifying build artifacts..."
echo "Build directory contents:"
run "ls -la build/"

echo "Checking for executables:"
test -f "${WORKSPACE_PATH}/build/cube23" && echo "✅ cube23 executable found" || echo "❌ cube23 not found"
test -f "${WORKSPACE_PATH}/build/cube23_vk" && echo "✅ cube23_vk executable found" || echo "❌ cube23_vk not found"
test -f "${WORKSPACE_PATH}/build/libvox.a" && echo "✅ libvox.a library found" || echo "❌ libvox.a not found"

# Verify shader compilation
echo "Checking for compiled shaders:"
if [ ! -d "${WORKSPACE_PATH}/build/shaders" ]; then
    echo "❌ ERROR: Shaders directory not found!"
    exit 1
fi

echo "Shaders directory exists, checking for required shader files:"
run "ls -la build/shaders/"

if [ ! -f "${WORKSPACE_PATH}/build/shaders/shader.vert.spv" ]; then
    echo "❌ ERROR: Vertex shader not compiled!"
    exit 1
fi

if [ ! -f "${WORKSPACE_PATH}/build/shaders/shader.frag.spv" ]; then
    echo "❌ ERROR: Fragment shader not compiled!"
    exit 1
fi

echo "✅ All required shaders compiled successfully"

# Test running cube23_vk executable
echo "🚀 Testing cube23_vk execution..."

case "$EXECUTION_MODE" in
    "mac_x11")
        if ! pgrep -f "XQuartz" > /dev/null; then
            echo "❌ ERROR: XQuartz not running."
            echo "   Start XQuartz and enable: Preferences > Security > ✓ Allow connections from network clients"
            exit 1
        fi

        if ! defaults read org.xquartz.X11 nolisten_tcp 2>/dev/null | grep -q "0"; then
            echo "⚠️  XQuartz network connections may not be enabled. Continuing anyway..."
        fi

        echo "🖥️  Running cube23_vk with X11 forwarding via XQuartz (you should see a window!)"
        
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
            -e CUBE23_TEST_MODE=1 \
            --add-host host.docker.internal:host-gateway \
            "${IMAGE_NAME}:${IMAGE_TAG}" \
            bash -c "cd build && timeout 10s ./cube23_vk"
        ;;
    "linux_x11")
        echo "🖥️  Running cube23_vk with X11 forwarding (you should see a window!)"
        
        if command -v xhost >/dev/null 2>&1; then
            xhost +local:docker > /dev/null 2>&1
        fi
        
        docker run --rm --platform linux/amd64 \
            -v "${WORKSPACE_PATH}:/workspace" \
            -e DISPLAY="${DISPLAY}" \
            -e CUBE23_TEST_MODE=1 \
            -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
            --network host \
            "${IMAGE_NAME}:${IMAGE_TAG}" \
            bash -c "cd build && timeout 10s ./cube23_vk"
        ;;
    "ci"|"docker_headless"|*)
        echo "🤖 Running cube23_vk headless with xvfb"
        run "cd build && CUBE23_TEST_MODE=1 xvfb-run -a --server-args=\"-screen 0 800x600x24\" bash -c './cube23_vk &
        sleep 2
        import -window root /workspace/screenshot.png
        wait'"
        ;;
esac

echo "✅ cube23_vk executed successfully"
echo "🎉 Build and execution tests completed successfully!"
echo "🏁 Test script completed successfully"

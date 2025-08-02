#!/bin/bash
set -e

# Test script for cube23 CI Docker environment
# 
# Usage:
#   Local development: ./test.sh
#     - Builds Docker image and runs full test suite
#   
#   CI environment: ./test.sh
#     - Uses pre-built image from workflow, runs tests only
#     - Automatically detects GitHub Actions environment

# Docker image configuration
IMAGE_NAME="cube23-ci"
IMAGE_TAG="latest"
DOCKERFILE="Dockerfile.ci"

# Determine workspace path and build command based on environment
if [ "${GITHUB_ACTIONS}" = "true" ]; then
    # Running in GitHub Actions CI environment
    WORKSPACE_PATH="${GITHUB_WORKSPACE}"
    BUILD_IMAGE=false
    echo "🔧 Running in GitHub Actions CI environment"
else
    # Running locally
    WORKSPACE_PATH="$(pwd)"
    BUILD_IMAGE=true
    echo "🏠 Running in local development environment"
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
echo "🎉 Build completed successfully!"
echo "🏁 Test script completed successfully"
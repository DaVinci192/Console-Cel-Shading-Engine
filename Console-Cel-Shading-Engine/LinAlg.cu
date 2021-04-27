#include <iostream>

#include <cuda_runtime.h>
#include <cassert>

using namespace std;
#include <vector>;
#include <algorithm>;

#include "device_launch_parameters.h"

#include <wchar.h>
#include <Windows.h>

/*
void setupTerminal(int width, int height, HANDLE& hCon)
{
	hCon = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	SetConsoleActiveScreenBuffer(hCon);
	SetConsoleScreenBufferSize(hCon, { width, height });
}

void drawPixel(int x, int y, int width, int height, HANDLE hCon)
{
	COORD pos = { x, y };
	wchar_t* unicode_array = new wchar_t[width * height];
	unicode_array[x + y * width] = 0x2584;
	wchar_t unicode_text[1] = { 0x2584 };
	LPCWSTR str = unicode_text;
	DWORD len = 1;
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacter(hCon, str, len, pos, &dwBytesWritten);
}
*/

// CUDA kernel for vector addition
// __global__ means this is called from the CPU and is run on the GPU
__global__ void vectorAdd(int* a, int* b, int* c, int N)
{
	// calculate global thread ID
	int tid = (blockIdx.x * blockDim.x) + threadIdx.x;

	// boundary check
	if (tid < N)
	{
		// Each thread adds a single element
		c[tid] = a[tid] + b[tid];
	}
}

// copied from https://github.com/CoffeeBeforeArch/cuda_programming/blob/master/vectorAdd/baseline/vectorAdd.cu
void verify_result(std::vector<int>& a, std::vector<int>& b,
	std::vector<int>& c) {
	for (int i = 0; i < a.size(); i++) {
		assert(c[i] == a[i] + b[i]);
	}
}

int test()
{
	// array size 2^16
	constexpr int N = 1 << 16;
	size_t bytes = sizeof(int) * N;

	// vectors holding host-side (CPU-bound) data
	vector<int> a(N);
	vector<int> b(N);
	vector<int> c(N);

	// initialize random numbers in each array
	std::generate(begin(a), end(a), []() {return rand() % 100; });
	std::generate(begin(b), end(b), []() {return rand() % 100; });

	// allocate memory on the device 
	int* d_a, * d_b, * d_c;
	cudaMalloc(&d_a, bytes);
	cudaMalloc(&d_b, bytes);
	cudaMalloc(&d_c, bytes);

	// copy data from host device (CPU -> GPU)
	cudaMemcpy(d_a, a.data(), bytes, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, b.data(), bytes, cudaMemcpyHostToDevice);

	// Threads per CTA (1024 threads per CTA)
	int NUM_THREADS = 1 << 10;

	// CTAs per Grid
	// We need to launch at least as many threads as we have elements
	// this equation pads an extra CTA to the grid if N cannot be evenly divided
	// by NUM_THREADS (e.g. N = 1025, NUM_THREADS = 1024)
	int NUM_BLOCKS = (N + NUM_THREADS - 1) / NUM_THREADS;

	// Launch the kernel on the GPU
	// Kernel calls are asnyc (the CPU program continues execution after the call, no
	// necessarily before the kernel finishes
	vectorAdd <<<NUM_BLOCKS, NUM_THREADS>>> (d_a, d_b, d_c, N);

	// copy sum vector from device to host 
	// cudaMemcpy is synchronous, waits for the prior kernel
	// launch to complete (both go to the default stream in this case).
	// Therefore cudaMemcpy acts as both a memcpy and synchronization barrier
	cudaMemcpy(c.data(), d_c, bytes, cudaMemcpyDeviceToHost);

	// check results for errors
	verify_result(a, b, c);

	// free memory on device
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);

	cout << "Completed Successfully" << endl;

	return 0;

}

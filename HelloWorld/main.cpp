#include <iostream>
#include <uv.h>
#include <memory>

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::shared_ptr<uv_loop_t> loop = std::make_shared<uv_loop_t>();
    uv_loop_init(loop.get());
    std::cout << "Now quitting.\n";
    uv_run(loop.get(), UV_RUN_DEFAULT);
    uv_loop_close(loop.get());

    return 0;
}

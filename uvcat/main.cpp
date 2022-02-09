#include <iostream>
#include <uv.h>
#include <cassert>

uv_fs_t open_req, read_req, write_req;
uv_buf_t iov;
char buffer[64];
void on_read(uv_fs_t *req);
// Due to the way filesystems and disk drives are configured for performance,
// a write that ‘succeeds’ may not be committed to disk yet.
void on_write(uv_fs_t *req){
    if (req->result < 0) {
        std::cerr << "write error: " << uv_strerror((int)req->result) << "\n";
    }
    else{
        uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov, 1, -1, on_read);
    }
}

void on_read(uv_fs_t *req){
    if (req->result < 0){
        std::cerr << "Read error: " << uv_strerror(req->result) << "\n";
    }
    else if (req->result == 0){ // EOF is indicated in this case by result being 0
        uv_fs_t close_req;
        // synchronous
        uv_fs_close(uv_default_loop(), &close_req, open_req.result, nullptr);
    }
    else if (req->result > 0){
        iov.len = req->result;
        uv_fs_write(uv_default_loop(), &write_req, 1, &iov, 1, -1, on_write);
    }
}

void on_open(uv_fs_t* req){
    // The request passed to the callback is the same as the one the call setup
    // function was passed.
    assert(req == &open_req);
    if (req->result >= 0){  // The result field of a uv_fs_t is the file descriptor in case of the uv_fs_open callback.
        iov = uv_buf_init(buffer, sizeof buffer);
        uv_fs_read(uv_default_loop(), &read_req, req->result, &iov, 1, -1, on_read);
    }
    else{
        std::cerr << "error opening file: " << uv_strerror((int)req->result) << '\n';
    }
}



int main(int argc, char** argv) {
    uv_fs_open(uv_default_loop(), &open_req, argv[1], O_RDONLY, 0, on_open);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    // The uv_fs_req_cleanup() function must always be called on filesystem requests to free internal memory allocations in libuv.
    uv_fs_req_cleanup(&open_req);
    uv_fs_req_cleanup(&read_req);
    uv_fs_req_cleanup(&write_req);
    return 0;
}

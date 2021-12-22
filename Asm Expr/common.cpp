#define MIN(x, y) ((x) <= (y) ? (x) : (y))
#define MAX(x, y) ((x) >= (y) ? (x) : (y))

#define ALIGN_DOWN(n, a) ((n) & ~((a) - 1))
#define ALIGN_UP(n, a) ALIGN_DOWN((n) + (a) - 1, (a))
#define ALIGN_DOWN_PTR(p, a) ((void *)ALIGN_DOWN((uintptr_t)(p), (a)))
#define ALIGN_UP_PTR(p, a) ((void *)ALIGN_UP((uintptr_t)(p), (a)))

const int ARENA_ALIGNMENT = 8;
const int ARENA_BLOCK_SIZE = 64*1024;

typedef unsigned char byte;

class Arena{
    std::vector<char*> blocks;
    char* ptr;
    char* end;
private:
    void grow(size_t min_size) {
        size_t size = ALIGN_UP(MAX(ARENA_BLOCK_SIZE, min_size), ARENA_ALIGNMENT);
        ptr = new char[size];
        end = ptr + size;
        blocks.push_back(ptr);
    }
public:
    Arena() {
        ptr = end = 0;
    }
    void* allocate(size_t size) {
        if (size > (size_t)(end - ptr)) {
            grow(size);
            assert(size <= (size_t)(end - ptr));
        }
        void* ptr = this->ptr;
        this->ptr = (char*)ALIGN_UP_PTR(this->ptr + size, ARENA_ALIGNMENT);
        assert(this->ptr <= this->end);
        assert(ptr == ALIGN_DOWN_PTR(ptr, ARENA_ALIGNMENT));
        return ptr;
    }
    template<typename T>
    inline T* alloc_type(){
        return static_cast<T*>(allocate(sizeof(T)));
    }
    void destroy() {
        for (auto e : blocks) {
            delete e;
        }
        blocks.clear();
    }
};

void* operator new(size_t size, Arena* arena) {
    return arena->allocate(size);
}

char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buf = static_cast<char*>(malloc(len + 1));
    if (len && fread(buf, len, 1, file) != 1) {
        fclose(file);
        free(buf);
        return NULL;
    }
    fclose(file);
    buf[len] = 0;
    return buf;
}
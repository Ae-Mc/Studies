#include <aio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define PATH_SIZE 200
#define CLUSTER_SIZE 512

using namespace std;
using namespace chrono;

enum OperationType { WRITE, READ };

struct AIOOperation {
    aiocb *aio;
    char *buffer;
    OperationType type;
    bool is_active;
    AIOOperation();
    AIOOperation(OperationType type, off_t cluster_size, off_t offset);
    ~AIOOperation();
};

struct Config {
    off_t cluster_size, operations_count;
    string in_path, out_path;
    off_t chunk_size();
    Config() = default;
    Config(const Config &) = delete;
    Config(int argc, char *argv[]);
};

int in_file_descriptor;
int out_file_descriptor;
off_t file_size;
Config config;

void init_operations(AIOOperation **&operations, int cluster);
void start_operations(AIOOperation **operations);
void wait_until_complete(AIOOperation **operations);
int write_results(string pathRead, int file_size, int operations_count,
                  int cluster, int duration);

int main(int argc, char *argv[]) {
    struct stat fileStat;
    config = Config(argc, argv);
    if (config.chunk_size() < 0) {
        cerr << "Error: too big chunk size (product of operations count and "
                "cluster "
                "size)"
             << endl;
    }
    AIOOperation **operations;

    in_file_descriptor =
        open(config.in_path.c_str(), O_RDONLY | O_NONBLOCK, 0666);
    out_file_descriptor = open(config.out_path.c_str(),
                               O_CREAT | O_WRONLY | O_TRUNC | O_NONBLOCK, 0666);

    if (in_file_descriptor == -1 || out_file_descriptor == -1) {
        cout << "Ошибка: Невозможно открыть файл!" << endl;
        return 1;
    }

    if (fstat(in_file_descriptor, &fileStat) == 1) {
        cout << "Ошибка: Нет доступа к информации!" << endl;
        close(in_file_descriptor);
        close(out_file_descriptor);
        return 1;
    }

    file_size = fileStat.st_size;
    cout << "Размер файла - " << file_size << endl;

    auto Start = high_resolution_clock::now();

    init_operations(operations, config.cluster_size);
    start_operations(operations);
    wait_until_complete(operations);

    auto end = high_resolution_clock::now();
    auto duration = end - Start;
    auto duration_ms = duration_cast<milliseconds>(duration);
    auto duration_u = duration_cast<microseconds>(duration);
    close(in_file_descriptor);
    close(out_file_descriptor);
    for (int i = 0; i < config.operations_count; i++) {
        delete operations[i];
    }
    delete[] operations;
    cout << "Копирование Завершено!" << endl;
    cout << "Затрачено времени: ";
    if (duration.count() < 10000) {
        cout << duration.count() << "ns" << endl;
    } else if (duration.count() < 10000000) {
        cout << duration_u.count() << "u" << endl;
    } else {
        cout << duration_ms.count() << "ms" << endl;
    }

    return write_results(config.in_path, file_size, config.operations_count,
                         config.cluster_size, duration_ms.count());
}

void aio_completion_handler(sigval_t sigval) {
    AIOOperation &aio_op = *(AIOOperation *)sigval.sival_ptr;

    if (aio_op.type == READ) {
        aio_op.type = WRITE;
        aio_op.aio->aio_fildes = out_file_descriptor;
        if (aio_op.aio->aio_offset + config.cluster_size > file_size) {
            aio_op.aio->aio_nbytes = file_size - aio_op.aio->aio_offset;
        }
        if (aio_write(aio_op.aio) == -1) {
            cerr << "Ошибка: асинхронная запись невозможна" << endl;
            aio_op.is_active = false;
            return;
        }
    } else if (aio_op.type == WRITE) {
        aio_op.aio->aio_offset += config.chunk_size();
        if (aio_op.aio->aio_offset >= file_size) {
            aio_op.is_active = false;
        } else {
            aio_op.type = READ;
            aio_op.aio->aio_fildes = in_file_descriptor;

            if (aio_read(aio_op.aio) == -1) {
                cerr << "Ошибка: асинхронное чтение невозможно" << endl;
                aio_op.is_active = false;
                return;
            }
        }
    } else {
        cerr << "Ошибка: неизвестный запрос" << endl;
        return;
    }
}

void init_operations(AIOOperation **&operations, int cluster) {
    operations = new AIOOperation *[config.operations_count]();
    for (off_t i = 0; i < config.operations_count; i++) {
        operations[i] = new AIOOperation(READ, cluster, cluster * i);
    }
}

void start_operations(AIOOperation **operations) {
    for (off_t i = 0; i < config.operations_count; i++) {
        if (operations[i]->aio != nullptr) {
            if (operations[i]->aio->aio_offset >= file_size) {
                operations[i]->is_active = false;
            } else {
                if (aio_read(operations[i]->aio) != -1) {
                    operations[i]->is_active = true;
                } else {
                    cerr << "Ошибка: Асинхронное чтение невозможно!" << endl;
                    cerr << aio_error(operations[i]->aio) << endl;
                    operations[i]->is_active = false;
                    return;
                }
            }
        }
    }
}

void wait_until_complete(AIOOperation **operations) {
    bool working;
    do {
        working = false;
        for (off_t i = 0; i < config.operations_count; i++) {
            if (operations[i]->is_active) {
                working = true;
                break;
            }
        }
        usleep(100);
    } while (working);
}

int write_results(string pathRead, int file_size, int operations_count,
                  int cluster, int duration) {
    ofstream results_file;
    results_file.open("results.csv", ios_base::app);
    if (!results_file.is_open()) {
        cerr << "Error writing results" << endl;
        return 1;
    }
    results_file << pathRead << ";" << file_size << ";" << operations_count
                 << ";" << cluster << ";" << duration << "\n";
    results_file.close();
    return 0;
}

off_t Config::chunk_size() { return operations_count * cluster_size; }

Config::Config(int argc, char *argv[]) {
    if (argc == 5) {
        vector<string> args(&argv[0], &argv[argc]);
        in_path = args[1];
        out_path = args[2];
        operations_count = atoi(args[3].c_str());
        cluster_size = atoi(args[4].c_str());
    } else {
        cout << "Введите путь к копируемому файлу: ";
        cin >> in_path;

        cout << "Введите путь к файлу записи: ";
        cin >> out_path;

        cout << "Введите количество операций: ";
        cin >> operations_count;

        cout << "Введите размер блока данных: ";
        cin >> cluster_size;
    }
}

AIOOperation::~AIOOperation() {
    if (buffer != nullptr) {
        delete[] buffer;
    }
    if (aio != nullptr) {
        delete aio;
    }
}

AIOOperation::AIOOperation()
    : aio(nullptr), buffer(nullptr), is_active(nullptr) {}

AIOOperation::AIOOperation(OperationType type, off_t buffer_size, off_t offset)
    : AIOOperation() {
    if (offset > file_size) {
        return;
    }

    if (aio == nullptr) {
        aio = new aiocb();
    }
    this->type = type;
    aio->aio_offset = offset;
    aio->aio_sigevent.sigev_notify = SIGEV_THREAD;
    aio->aio_sigevent.sigev_notify_function = aio_completion_handler;
    aio->aio_sigevent.sigev_value.sival_ptr = this;
    aio->aio_fildes = in_file_descriptor;

    aio->aio_nbytes = buffer_size;
    buffer = new char[aio->aio_nbytes];
    aio->aio_buf = buffer;
}

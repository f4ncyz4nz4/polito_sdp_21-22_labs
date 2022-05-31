#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <queue>
#include <iostream>

std::mutex m;
std::condition_variable adminCV, adderCV;
//std::queue<int> taskQueue;
int var = 0;

void admin_f() {
    std::unique_lock<std::mutex> lock{m};
    var = 10;
    adminCV.wait(lock);
    std::cout << "Final value: " << var << std::endl;
}

void adder_f() {
    std::unique_lock<std::mutex> lock{m};
    int add_unit = rand() % 6;
    var += add_unit;
    if (var >= 15)
        adminCV.notify_one();
    lock.unlock();
}

int main() {
    int var = 0;
    std::vector<std::thread> adders;
    std::thread admin(admin_f);
    for (int i = 0; i < 3; i++) {
        srand((unsigned) time(NULL)); //makes seed different for calling rand()
        adders.emplace_back(std::thread(adder_f));
    }
    for (auto &i: adders) {
        i.join();
    }
    adminCV.notify_one();
    admin.join();
    return 0;
}
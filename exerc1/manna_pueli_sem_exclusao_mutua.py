import threading
import time
import random

num_threads = 4
sum = 0

request = 0
respond = 0

def client():
    global respond
    global sum
    global request
    while(True):
        tid = threading.get_native_id()
        # while(respond != tid):
        request = tid
        soma_local = sum
        time.sleep(random.random()%2)
        sum = soma_local + 1

        print(f'Thread {tid}, soma = {sum}')

        respond = 0


def server():
    global respond
    global request
    while(True):
        while(request == 0):
            continue
        respond = request
        while(respond != 0):
            continue
        request = 0


if __name__ == "__main__":
    thread_server = threading.Thread(target=server)
    thread_server.start()

    threads = []
    for i in range(num_threads):
        t = threading.Thread(target=client)
        t.start()
        threads.append(t)

    for t in threads:
        t.join()

    thread_server.join()
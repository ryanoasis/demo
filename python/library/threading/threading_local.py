import random
import threading
import logging

logging.basicConfig(level=logging.DEBUG,
        format='(%(threadName)-10s) %(message)s')

def show_value(data):
    try:
        val = data.value
    except AttributeError:
        logging.debug('No value yet')
    else:
        logging.debug('value=%s', val)

def worker(data):
    show_value(data)
    data.value = random.randint(1, 100)
    show_value(data)

local_data = threading.local()
show_value(local_data)
local_data.value = 1000
show_value(local_data)


for i in range(2):
    t = threading.Thread(target=worker, args=(local_data,))
    t.start()

"""
(MainThread) No value yet
(MainThread) value=1000
(Thread-1  ) No value yet
(Thread-1  ) value=26
(Thread-2  ) No value yet
(Thread-2  ) value=79
"""

# A Timer starts its work after a delay, and can be canceled at any point within
# that delay time period.

import threading
import time
import logging

logging.basicConfig(level=logging.DEBUG,
        format='(%(threadName)-10s) %(message)s')

def delayed():
    logging.debug('worker running')

t1 = threading.Timer(3, delayed)
t1.setName('t1')
t2 = threading.Timer(3, delayed)
t2.setName('t2')

logging.debug('starting timers')
t1.start()
t2.start()

logging.debug('waiting before canceling %s', t2.getName())
time.sleep(2)
logging.debug('canceling %s', t2.getName())
t2.cancel()
logging.debug('done')

"""
(MainThread) starting timers
(MainThread) waiting before canceling t2
(MainThread) canceling t2
(MainThread) done
(t1        ) worker running
"""
# To see what signal handlers are registered for a signal, use getsignal().

import signal

def alarm_received(n, stack):
    return

signal.signal(signal.SIGALRM, alarm_received)

signals_to_names = {}
for n in dir(signal):
    if n.startswith('SIG') and not n.startswith('SIG_'):
        signals_to_names[getattr(signal, n)] = n

for s, name in sorted(signals_to_names.items()):
    handler = signal.getsignal(s)
    if handler is signal.SIG_DFL:
        handler = 'SIG_DFL'
    elif handler is signal.SIG_IGN:
        handler = 'SIG_IGN'
    print '%-10s (%2d):' % (name, s), handler

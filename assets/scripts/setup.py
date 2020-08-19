import memelib as ml

# messages
print(f'# {ml.prj.name} | {ml.prj.arch!r}-bit | {ml.prj.config}')
print(f'# {ml.prj.url}')
print('# type \'help\' for a list of commands')
print()

# memory
print('TEST MEMORY')
s = 'Hello, World!'
p = ml.strcpy(ml.malloc(len(s) + 1), s)
r = ml.memrec(p)
print(r)
print(ml.memstr(p))
print(ml.memstr(p, p + r.size))
print(chr(ml.memget(p, 1)))
print(chr(ml.memget(p + r.size - 2)))
print(ml.memget(p, 1, r.size))
ml.free(p)
print(ml.memrec(p))
print()

# json
print('TEST JSON')
class foo:
    def __init__(self):
        self.bar = 123
        self.baz = 'abc'
j = ml.json(foo())
print(j)
print(j['bar'])
print(j['baz'])
j['test'] = 1.4
print(j['test'])
print(j)
print()
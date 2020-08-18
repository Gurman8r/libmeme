import ml_ml    as ml
import ml_app   as app

print(f"# {ml.cfg.name} | {ml.cfg.arch!r}-bit | {ml.cfg.configuration}")
print(f"# {ml.cfg.url}")
print("# type \'help\' for a list of commands")
print("")

a = ml.mem_alloc(100)
ml.mem_set(a, 1, 100)
print(str(ml.mem_get(a + 1)))
ml.mem_free(a)

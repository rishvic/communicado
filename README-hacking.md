# Communicado

## Development 👨‍💻

While development, you might need a `compile_commands.json` to help with
editor autocomplete and such. For getting the file, instead of running the usual
`cmake ..`, you can run:

```shell
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
```

This will generate a `compile_commands.json` inside the build directory, which
you can copy into the root of the repository.

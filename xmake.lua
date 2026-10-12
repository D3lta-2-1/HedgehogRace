add_rules("mode.debug", "mode.release", "mode.releasedbg")

target("HedgehogRace")
    set_languages("c11")
    set_kind("binary")
    add_files("src/*.c")
    remove_files("src/test.c")
    add_includedirs("include")
    set_warnings("all", "error")


target("test")
    set_languages("c11")
    set_kind("binary")
    add_files("src/*.c")
    remove_files("src/main.c")
    add_includedirs("include")
    set_warnings("all")

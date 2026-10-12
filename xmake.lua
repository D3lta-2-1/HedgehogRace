add_rules("mode.debug", "mode.release", "mode.releasedbg")

if is_mode("debug") then
    set_policy("build.sanitizer.address", true)
end

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

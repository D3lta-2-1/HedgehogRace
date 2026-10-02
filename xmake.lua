add_rules("mode.debug", "mode.release")

target("HedgehogRace")
    set_languages("c11")
    set_kind("binary")
    add_files("src/*.c")
    add_includedirs("include")

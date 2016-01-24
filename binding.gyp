{
    "targets": [
        {
            "target_name": "ons",
            "sources": [
                "src/entry.cpp",
                "src/consumer_ack.cpp",
                "src/consumer.cpp",
                "src/producer.cpp"
            ],
            "include_dirs": [
                "src/third_party/include",
                "<!(node -e \"require('nan')\")"
            ],
            "libraries": [
                "<!(node -e \"console.log(process.cwd() + '/src/third_party/lib/libonsclient4cpp.a')\")"
            ],
            "cflags_cc!": [ "-fno-exceptions", "-pthread", "-Wl,--no-as-needed", "-ldl" ]
        }
    ]
}

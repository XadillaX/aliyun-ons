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

            "conditions": [
                ["OS==\"max\"", {
                    "cflags_cc!": [ "-fno-exceptions", "-pthread", "-Wl,--no-as-needed", "-ldl" ],
                    "cflags_cc": [ "-Wno-ignored-qualifiers" ]
                }],
                ["OS==\"linux\"", {
                    "libraries": [
                        "../src/third_party/lib/linux/libonsclient4cpp.a"
                    ],
                    "cflags_cc!": [ "-fno-exceptions", "-pthread", "-Wl,--no-as-needed", "-ldl" ],
                    "cflags_cc": [ "-Wno-ignored-qualifiers" ]
                }],
                ["OS==\"win\"", {
                    "conditions": [
                        ["target_arch==\"ia32\"", {
                            "libraries": [
                                "../src/third_party/lib/windows/32x/ONSClient4CPP.lib"
                            ],
                            "copies": [
                                {
                                    "destination": "<(module_root_dir)/build/Release/",
                                    "files": [ "<(module_root_dir)/src/third_party/lib/windows/32x/ONSClient4CPP.dll" ]
                                }
                            ]
                        }],
                        ["target_arch==\"x64\"", {
                            "libraries": [
                                "../src/third_party/lib/windows/64x/ONSClient4CPP.lib"
                            ],
                            "copies": [
                                {
                                    "destination": "<(module_root_dir)/build/Release/",
                                    "files": [ "<(module_root_dir)/src/third_party/lib/windows/64x/ONSClient4CPP.dll" ]
                                }
                            ]
                        }]
                    ]
                }]
            ]
        }
    ]
}

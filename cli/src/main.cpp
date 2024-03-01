#include <stdio.h>
#include <iostream>
#include <optional>
#include <pdif_cli/pdif_cli_config.hpp>
#include <pdif/pdif_engine_config.hpp>
#include <pdif/pdf.hpp>
#include <pdif/lcs_stream_differ.hpp>

void print_version()
{
    printf("pdif_cli version %s.%s.%s\n", PDIF_CLI_VERSION_MAJOR, PDIF_CLI_VERSION_MINOR, PDIF_CLI_VERSION_PATCH);
    printf("pdif_engine version %s.%s.%s\n", PDIF_ENGINE_VERSION_MAJOR, PDIF_ENGINE_VERSION_MINOR, PDIF_ENGINE_VERSION_PATCH);
}

struct args {
    std::string command;
    std::string file1;
    std::string file2;
    std::optional<std::string> output_file;
    bool compare_meta = false;
    bool compare_text = false;
    bool compare_image = false;
};

void print_usage()
{
    printf("usage: pdif [diff|apply|help|version] [options] <file1> <file2>");
    printf("  diff: compare two PDF files and output the differences\n");
    printf("  apply: apply a diff to a PDF file\n");
    printf("  help: print this message\n");
    printf("  version: print the version of pdif_cli\n");

    printf("  options:\n");
    printf("    -o, --output <file>: the output file\n");
    printf("    -m, --meta: compare only the meta data\n");
    printf("    -t, --text: compare only the text\n");
    printf("    -i, --image: compare only the images\n");
}

args parse_arguments(int argc, char *argv[]) {
    args a;

    if (argc < 2) {
        print_usage();
        exit(1);
    }

    a.command = argv[1];

    if (a.command == "version") {
        print_version();
        exit(0);
    }

    if (a.command == "help") {
        print_usage();
        exit(0);
    }

    if (argc < 4) {
        print_usage();
        exit(1);
    }

    a.file1 = argv[argc - 2];
    a.file2 = argv[argc - 1];


    if (a.command != "diff" && a.command != "apply") {
        print_usage();
        exit(1);
    }

    for (int i = 2; i < argc - 2; ++i) {
        std::string arg = argv[i];
        if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc - 2) {
                a.output_file = argv[i + 1];
                ++i; // Skip the next argument
            } else {
                std::cerr << "Error: Missing argument for output file\n";
                print_usage();
                exit(1);
            }
        } else if (arg == "-m" || arg == "--meta") {
            if (a.command != "diff") {
                std::cerr << "Error: Option --meta is only available for the diff command\n";
                print_usage();
                exit(1);
            }
            a.compare_meta = true;
        } else if (arg == "-t" || arg == "--text") {
            a.compare_text = true;
            if (a.command != "diff") {
                std::cerr << "Error: Option --text is only available for the diff command\n";
                print_usage();
                exit(1);
            }
        } else if (arg == "-i" || arg == "--image") {
            a.compare_image = true;
            if (a.command != "diff") {
                std::cerr << "Error: Option --image is only available for the diff command\n";
                print_usage();
                exit(1);
            }
        } else {
            std::cerr << "Error: Unknown option '" << arg << "'\n";
            print_usage();
            exit(1);
        }
    }

    if (!a.compare_meta && !a.compare_text && !a.compare_image) {
        a.compare_meta = true;
        a.compare_text = true;
        a.compare_image = true;
    }

    return a;
}

int main(int argc, char** argv)
{
    args a = parse_arguments(argc, argv);

    if (a.command == "diff") {
        pdif::PDF file1(a.file1);
        pdif::PDF file2(a.file2);

        pdif::PDF::comparison_args args;
        args.compare_meta = a.compare_meta;
        args.compare_text = a.compare_text;
        args.compare_image = a.compare_image;

        pdif::diff  diff = file1.compare<pdif::lcs_stream_differ>(file2, args);

        if (a.output_file.has_value()) {
            // std::ofstream ofs(a.output_file.value());
            // ofs << diff.to_json();
            // ofs.close();
        } else {
            diff.output_meta_edit_script(std::cout);
            std::cout << std::endl;
            diff.output_edit_script(std::cout);
            std::cout << std::endl;
        }
    } else if (a.command == "apply") {
    }

    return 0;
}
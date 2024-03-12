#include <stdio.h>
#include <iostream>
#include <fstream>
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
    std::optional<std::string> spacing;
    bool meta_only = false;
    bool content_only = false;
    bool summary = false;
    pdif::scope scope = pdif::scope::page;
    pdif::granularity granularity = pdif::granularity::word;
    bool write_console_colors = true;
    int pageno = -1;
    int context_lines = 3;
};

void print_usage()
{
    printf("usage: pdif [diff|extract|help|version]\n");
    printf("  diff [diff_options] <pdf1> <pdf2>: compare two PDF files and output the differences\n");
    printf("  extract [extract_options] <file>: extract the content of a PDF file\n");
    printf("  help: print this message\n");
    printf("  version: print the version of pdif_cli\n");

    printf("  diff_options:\n");
    printf("    -o, --output <file>: the output file\n");
    printf("    -m, --meta: compare only the meta data\n");
    printf("    -C, --content: compare only stream contents\n");
    printf("    -s, --scope <page|document>: the scope of the comparison\n");
    printf("    -S, --summary: output the number of additions, deletions and eqs\n");
    printf("    -g, --granularity <letter|word|sentence>: the granularity of the comparison\n");
    printf("    -p, --page <number>: the page number to extract (negative for all, 0 for meta) \n");
    printf("    -n, --no-color: do not use console colors in the output\n");
    printf("    -c, --context <number>: the number of context lines to show\n");
    printf("\n");
    printf("   extract_options:\n");
    printf("    -g, --granularity <letter|word|sentence>: the granularity of the extraction\n");
    printf("    -p, --page <number>: the page number to extract (negative for all, 0 for meta) \n");
    printf("    -o, --output <file>: the output file\n");
    printf("    -s, --spacing <value>: the spacing between the elements\n");
    printf("    -n, --no-color: do not use console colors in the output\n");
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

    if (a.command == "extract") {
        for (int i = 2; i < argc - 1; i++) {
            std::string arg = argv[i];
            if (arg == "-g" || arg == "--granularity") {
                if (i + 1 < argc - 1) {
                    std::string granularity = argv[i + 1];
                    if (granularity != "letter" && granularity != "word" && granularity != "sentence") {
                        std::cerr << "Error: Invalid granularity '" << granularity << "'\n";
                        print_usage();
                        exit(1);
                    }
                    if (granularity == "letter") {
                        a.granularity = pdif::granularity::letter;
                    } else if (granularity == "word") {
                        a.granularity = pdif::granularity::word;
                    } else {
                        a.granularity = pdif::granularity::sentence;
                    }
                    ++i; // Skip the next argument
                } else {
                    std::cerr << "Error: Missing argument for granularity\n";
                    print_usage();
                    exit(1);
                }
            } else if (arg == "-p" || arg == "--page") {
                if (i + 1 < argc - 1) {
                    a.pageno = std::stoi(argv[i + 1]);
                    ++i; // Skip the next argument
                } else {
                    std::cerr << "Error: Missing argument for page number\n";
                    print_usage();
                    exit(1);
                }
            } else if (arg == "-s" || arg == "spacing") {
                if (i + 1 < argc - 1) {
                    a.spacing = argv[i + 1];
                    i++;
                } else {
                    std::cerr << "Error: Missing argument for spacing\n";
                    print_usage();
                    exit(1);
                }
            } else if (arg == "-o" || arg == "--output") {
                if (i + 1 < argc - 1) {
                    a.output_file = argv[i + 1];
                    ++i; // Skip the next argument
                } else {
                    std::cerr << "Error: Missing argument for output file\n";
                    print_usage();
                    exit(1);
                }
            } else if (arg == "-n" || arg == "--no-color") {
                a.write_console_colors = false;
            } else {
                std::cerr << "Error: Unknown option '" << arg << "'\n";
                print_usage();
                exit(1);
            }
        }

        a.file1 = argv[argc - 1];
        return a;
    }

    if (argc < 4) {
        print_usage();
        exit(1);
    }

    a.file1 = argv[argc - 2];
    a.file2 = argv[argc - 1];


    if (a.command != "diff") {
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
        } else if (arg == "-s" || arg == "--scope") {
            if (i + 1 < argc - 2) {
                std::string scope = argv[i + 1];
                if (scope != "page" && scope != "document") {
                    std::cerr << "Error: Invalid scope '" << scope << "'\n";
                    print_usage();
                    exit(1);
                }
                if (scope == "page") {
                    a.scope = pdif::scope::page;
                } else {
                    a.scope = pdif::scope::document;
                }
                ++i; // Skip the next argument
            } else {
                std::cerr << "Error: Missing argument for scope\n";
                print_usage();
                exit(1);
            }
        } else if (arg == "-g" || arg == "--granularity") {
            if (i + 1 < argc - 2) {
                std::string granularity = argv[i + 1];
                if (granularity != "letter" && granularity != "word" && granularity != "sentence") {
                    std::cerr << "Error: Invalid granularity '" << granularity << "'\n";
                    print_usage();
                    exit(1);
                }
                if (granularity == "letter") {
                    a.granularity = pdif::granularity::letter;
                } else if (granularity == "word") {
                    a.granularity = pdif::granularity::word;
                } else {
                    a.granularity = pdif::granularity::sentence;
                }
                ++i; // Skip the next argument
            } else {
                std::cerr << "Error: Missing argument for granularity\n";
                print_usage();
                exit(1);
            }
        } else if (arg == "-c" || arg == "--context") {
            if (i + 1 < argc - 2) {
                a.context_lines = std::stoi(argv[i + 1]);

                if (a.context_lines < 0) {
                    std::cerr << "Error: Invalid context lines '" << a.context_lines << "'\n";
                    print_usage();
                    exit(1);
                }
                i++;
            } else {
                std::cerr << "Error: Missing argument for context lines\n";
                print_usage();
                exit(1);
            }
        } else if (arg == "-p" || arg == "--page") {
            if (i + 1 < argc - 1) {
                a.pageno = std::stoi(argv[i + 1]);
                if (a.pageno == 0) {
                    std::cerr << "Error: page number can either be less than 0 (all) or greater than 0 (page number)\n";
                    print_usage();
                    exit(1);
                }
                ++i; // Skip the next argument
            } else {
                std::cerr << "Error: Missing argument for page number\n";
                print_usage();
                exit(1);
            }
        } else if (arg == "-n" || arg == "--no-color") {
            a.write_console_colors = false;
        } else if (arg == "-m" || arg == "--meta") {
            a.meta_only = true;
        } else if (arg == "-C" || arg == "--content") {
            a.content_only = true;
        } else if (arg == "-S" || arg == "--summary") {
            a.summary = true;
        } else {
            std::cerr << "Error: Unknown option '" << arg << "'\n";
            print_usage();
            exit(1);
        }

    }

    if (!a.meta_only && !a.content_only) {
        a.meta_only = true;
        a.content_only = true;
    }

    return a;
}

int main(int argc, char** argv)
{
    args a = parse_arguments(argc, argv);

    if (a.command == "diff") {
        pdif::PDF file1(a.file1, a.granularity, a.scope, a.write_console_colors, a.pageno - 1);
        pdif::PDF file2(a.file2, a.granularity, a.scope, a.write_console_colors, a.pageno - 1);

        pdif::diff diff = file1.compare<pdif::lcs_stream_differ>(file2);
        diff.set_allowed_context(a.context_lines);

        std::ofstream ofs;
        std::ostream *output;

        if (a.output_file.has_value()) {
            ofs = std::ofstream(a.output_file.value());
            output = &ofs;
        } else {
            output = &std::cout;
        }

        if (a.meta_only) {
            diff.output_meta_edit_script(*output);
            if (a.summary) {
                diff.output_meta_summary(*output);
            }

            *output << std::endl;
        }

        if (a.content_only) {
            diff.output_edit_script(*output);

            if (a.summary) {
                diff.output_edit_summary(*output);
            }
        }

        if (a.output_file.has_value()) {
            ofs.close();
        }

    } else if (a.command == "extract") {
        pdif::PDF file(a.file1, a.granularity, pdif::scope::page, a.write_console_colors, a.pageno - 1);

        std::ofstream ofs;
        std::ostream *output;

        if (a.output_file.has_value()) {
            ofs = std::ofstream(a.output_file.value());
            output = &ofs;
        } else {
            output = &std::cout;
        }

        if (a.pageno == 0) {
            file.dump_meta(*output);
        } else {
            file.dump_content(*output, a.spacing);
        }

        if (a.output_file.has_value()) {
            ofs.close();
        }
    }

    return 0;
}
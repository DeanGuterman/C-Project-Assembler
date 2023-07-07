

int main(int argc, char* argv[]){
    char* parsed_source_files = parse_macros(argc, argv);
    char* first_pass_products = first_pass_through(argc, parsed_source_files);
    char* second_pass_products = second_pass_through(argc, first_pass_products);
    return 0;
}


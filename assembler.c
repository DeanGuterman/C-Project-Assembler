int parse_macros(char* argv);
int first_pass_through(char* argv);

int main(int argc, char* argv[]){
    int i;
    for (i = 1; i < argc; i++){
        parse_macros(argv[i]);
        first_pass_through(argv[i]);
    }
    return 0;
}


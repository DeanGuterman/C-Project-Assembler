int parse_macros(char* argv);

int main(int argc, char* argv[]){
    int i;
    for (i = 1; i < argc; i++){
        parse_macros(argv[i]);
    }
    return 0;
}


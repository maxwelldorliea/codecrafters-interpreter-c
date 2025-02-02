#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum TokenType {
  IDENTIFIER,
  STRING,
  EOFL,
  LEFT_PAREN,
  RIGHT_PAREN,
  RIGHT_BRACE,
  LEFT_BRACE,
  STAR,
  DOT,
  COMMA,
  PLUS,
  MINUS,
  SEMICOLON,
  EQUAL,
  EQUAL_EQUAL,
  BANG,
  BANG_EQUAL,
  LESS,
  LESS_EQUAL,
  GREATER,
  GREATER_EQUAL,
  COMMENT,
  SLASH,
  AND,
  IF,
  NIL,
  CLASS,
  ELSE,
  FALSE,
  FOR,
  FUN,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE
};


typedef struct Token_s {
  enum TokenType type;
  char* lexeme;
  void* literal;
} Token;

char* strToUpper(char *s) {
    char *str = strdup(s);
    for (char *p = str; *p; ++p) {
        *p = toupper((unsigned char)*p);
    }
  return str;
}


int isReserved(char *s) {
// and, class, else, false, for, fun, if, nil, or, print, return, super, this, true, var, while
  Token reserves[] = {
    { AND, "and", "null"},
    { CLASS, "class", "null"},
    { ELSE, "else", "null"},
    { IF, "if", "null"},
    { FALSE, "false", "null"},
    { FOR, "for", "null"},
    { FUN, "fun", "null"},
    { NIL, "nil", "null"},
    { OR, "or", "null"},
    { PRINT, "print", "null"},
    { RETURN, "return", "null"},
    { SUPER, "super", "null"},
    { THIS, "this", "null"},
    { TRUE, "true", "null"},
    { VAR, "var", "null"},
    { WHILE, "while", "null"},
  };
  int resLen = 16;

  for (int i = 0; i < resLen; i++) {
    if (strcmp(reserves[i].lexeme, s) == 0) {
      char *token = strToUpper(reserves[i].lexeme);
      printf("%s %s null\n", token, reserves[i].lexeme);
      free(token);
      return 1;
    }
  }

  return 0;
}

char *read_file_contents(const char *filename);

int scanStr(char *s, int start, int end, int line, int* hasError) {
  char str[100];
  int i = 0, found = 0;
  while (start < end) {
    char c = s[start];
    if (c == '\n') break;
    if (c == '"') {
      str[i] = '\0';
      printf("STRING \"%s\" %s\n", str, str);
      found = 1;
      break;
    }
    str[i++] = c;
    start++;
  }
  i++;
  if (!found) {
    fprintf(stderr, "[line %d] Error: Unterminated string.\n", line);
    *hasError = 1;
    return i;
  }
  return i;
}

int scanNum(char *s, int start, int end, int line, int* hasError) {
  char str[100];
  char *derr;
  int i = 0, found = 0, deci = 0, hasDeci = 0, dAllZero = 0;
  while (start <= end) {
    char c = s[start];
    if (c == ' ' || c == '\n' || start == end) {
      str[i] = '\0';
      if (!deci) deci = 1;
      if (dAllZero) deci -= dAllZero - 1;
      printf("NUMBER %s %.*f\n", str, deci, strtod(str, &derr));
      found = 1;
      break;
    }
    if (hasDeci) deci++;
    if (hasDeci && c == '0') dAllZero++;
    else dAllZero = 0;
    if (c == '.') hasDeci = 1;
    if (isdigit(c) || (c == '.' && isdigit(str[0]))) {
      str[i++] = c;
    } else {
      if (i) {
        str[i] = '\0';
        if (!deci) deci = 1;
        if (dAllZero) deci -= dAllZero - 1;
        printf("NUMBER %s %.*f\n", str, deci, strtod(str, &derr));
        return i - 1;
      }
    }
    start++;
  }
  return i;
}

int scanIdentifier(char *s, int start, int end) {
  char str[100];
  int i = 0;

  while (start <= end) {
    char c = s[start];
    if (c == ' ' || c == '\n' || start == end) {
      str[i] = '\0';
      if(!isReserved(str)) printf("IDENTIFIER %s null\n", str);
      break;
    }
    if (isalnum(c) || c == '_') str[i++] = c;
    start++;
  }
  return i;
}

int main(int argc, char *argv[]) {
    // Disable output buffering
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    if (argc < 3) {
        fprintf(stderr, "Usage: ./your_program tokenize <filename>\n");
        return 1;
    }

    const char *command = argv[1];

    if (strcmp(command, "tokenize") == 0) {
        // You can use print statements as follows for debugging, they'll be visible when running tests.
        fprintf(stderr, "Logs from your program will appear here!\n");

        char *file_contents = read_file_contents(argv[2]);

        // if (strlen(file_contents) > 0) {
        //     fprintf(stderr, "Scanner not implemented\n");
        //     exit(1);
        // }
        int i = 0, line = 1;
        int hasError = 0;
        int isComment = 0;
        while (i < strlen(file_contents)) {
          if (isComment) {
            if (file_contents[i] == '\n') {
              isComment = 0;
              line++;
            }
            i++;
            continue;
          }
          if (file_contents[i] == '\t' || file_contents[i] == ' ') {
            i++;
            continue;
          }
          if (isalpha(file_contents[i]) || file_contents[i] == '_') {
            i += scanIdentifier(file_contents, i, strlen(file_contents));
            continue;
          }
          switch (file_contents[i]) {
            case '\n':
              line++;
              break;
            case ')':
              printf("RIGHT_PAREN %c null\n", file_contents[i]);
              break;
            case '(':
              printf("LEFT_PAREN %c null\n", file_contents[i]);
              break;
            case '{':
              printf("LEFT_BRACE %c null\n", file_contents[i]);
              break;
            case '}':
              printf("RIGHT_BRACE %c null\n", file_contents[i]);
              break;
            case '-':
              printf("MINUS %c null\n", file_contents[i]);
              break;
            case '+':
              printf("PLUS %c null\n", file_contents[i]);
              break;
            case '*':
              printf("STAR %c null\n", file_contents[i]);
              break;
            case '.':
              printf("DOT %c null\n", file_contents[i]);
              break;
            case ',':
              printf("COMMA %c null\n", file_contents[i]);
              break;
            case ';':
              printf("SEMICOLON %c null\n", file_contents[i]);
              break;
            case '=':
              if (file_contents[i+1] == '=') {
                printf("EQUAL_EQUAL =%c null\n", file_contents[i + 1]);
                i++;
              } else {
                printf("EQUAL %c null\n", file_contents[i]);
              }
              break;
            case '!':
              if (file_contents[i+1] == '=') {
                printf("BANG_EQUAL %c= null\n", file_contents[i]);
                i++;
              } else {
                printf("BANG %c null\n", file_contents[i]);
              }
              break;
            case '>':
              if (file_contents[i+1] == '=') {
                printf("GREATER_EQUAL %c= null\n", file_contents[i]);
                i++;
              } else {
                printf("GREATER %c null\n", file_contents[i]);
              }
              break;
            case '<':
              if (file_contents[i+1] == '=') {
                printf("LESS_EQUAL %c= null\n", file_contents[i]);
                i++;
              } else {
                printf("LESS %c null\n", file_contents[i]);
              }
              break;
            case '/':
              if (file_contents[i+1] == '/') {
                isComment = 1;
                i++;
              } else {
                printf("SLASH %c null\n", file_contents[i]);
              }
              break;
            case '"':
              i += scanStr(file_contents, i + 1, strlen(file_contents), line, &hasError);
              break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
              i += scanNum(file_contents, i, strlen(file_contents), line, &hasError);
              break;
            default:
              fprintf(stderr, "[line %d] Error: Unexpected character: %c\n", line, file_contents[i]);
              hasError = 1;
              break;

          }
          i++;
        }
        printf("EOF  null\n"); // Placeholder, remove this line when implementing the scanner
        free(file_contents);
        if (hasError) return 65;
    } else {
        fprintf(stderr, "Unknown command: %s\n", command);
        return 1;
    }

    return 0;
}

char *read_file_contents(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error reading file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *file_contents = malloc(file_size + 1);
    if (file_contents == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(file_contents, 1, file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "Error reading file contents\n");
        free(file_contents);
        fclose(file);
        return NULL;
    }

    file_contents[file_size] = '\0';
    fclose(file);

    return file_contents;
}

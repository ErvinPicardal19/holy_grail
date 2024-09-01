#include "configs.h"

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char project_name[64];

void set_project_name(char *name)
{
    memset(project_name, 0, 64);
    strcpy(project_name, name);
}

void create_config(const char *project_dir, Config config)
{
    FILE       *file;
    const char *output;
    char type[64];
    char        temp_file_holder[PATH_MAX];
    memset(temp_file_holder, 0, PATH_MAX);
    memset(type, 0, 64);
    strcat(temp_file_holder, project_dir);
    switch (config) {
        case MAKEFILE:
            strcat(temp_file_holder, "/Makefile");
            char temp[4096];
            memset(temp, 0, sizeof(temp));
            strcat(temp, "EXEC := ");
            strcat(temp, project_name);
            strcat(temp, "\n");
            strcat(temp, makefile_template);
            output = temp;
            strcpy(type, "Makefile");
            break;
        case CLANG_FORMAT:
            strcat(temp_file_holder, "/.clang-format");
            output = clang_format_config;
            strcpy(type, ".clang-format");
            break;
        case CLANG_D:
            strcat(temp_file_holder, "/.clangd");
            output = clangd_config;
            strcpy(type, ".clangd");
            break;
        case CLANG_TIDY:
            strcat(temp_file_holder, "/.clang-tidy");
            output = clang_tidy_config;
            strcpy(type, ".clang-tidy");
            break;
        default:
            fprintf(stderr, "Invalid config %d\n", config);
            exit(1);
            break;
    }

    file = fopen(temp_file_holder, "w");

    if (file == NULL) {
        fprintf(stderr, "Cannot create %s\n", temp_file_holder);
        perror("fopen");
        exit(1);
    }

    fprintf(file, "%s", output);

    fclose(file);

    fprintf(stdout, "\033[32m%s created.\033[0m\n", type);
}

void create_project_dir(char *project_dir)
{
    FILE *file;
    char  src_dir[PATH_MAX];
    char  include_dir[PATH_MAX];
    memset(src_dir, 0, PATH_MAX);
    memset(include_dir, 0, PATH_MAX);

    strcat(src_dir, project_dir);
    strcat(include_dir, project_dir);
    strcat(src_dir, "/src");
    strcat(include_dir, "/include");

    if ((mkdir(project_dir, S_IRWXU | S_IRWXG)) == -1 ||
        (mkdir(src_dir, S_IRWXU | S_IRWXG)) == -1 ||
        (mkdir(include_dir, S_IRWXU | S_IRWXG)) == -1) {
        fprintf(stderr, "Cannot create project directory.\n");
        perror("mkdir: ");
        exit(1);
    }

    strcat(src_dir, "/main.c");
    file = fopen(src_dir, "w");

    if (file == NULL) {
        fprintf(stderr, "Cannot create %s\n", src_dir);
        perror("fopen");
        exit(1);
    }

    fprintf(file, "%s", main_file);

    fclose(file);
}
const char *makefile_template =
    "debug ?= 1\n"
    "\n"
    "# Directories\n"
    "LIB_DIR := lib\n"
    "BIN_DIR := bin\n"
    "BUILD_DIR := build\n"
    "SRC_DIR := src\n"
    "INCLUDE_DIR := include\n"
    "\n"
    "# Project Filenames\n"
    "EXEC ?= test\n"
    "OBJS := $(patsubst %.c,%.o,$(wildcard ${SRC_DIR}/*.c))\n"
    "HEADERS := $(wildcard ${INCLUDE_DIR}/*.h)\n"
    "SO_FILE := lib$(EXEC).so\n"
    "\n"
    "ifeq ($(strip $(OBJS)),)\n"
    "\tERROR_MESSAGE := \"No source file to compile\"\n"
    "endif\n"
    "\n"
    "# Compiler Settings\n"
    "\n"
    "# Compiler and Linker Flags\n"
    "CC ?= gcc\n"
    "FLAGS := -Wall -Werror -pedantic\n"
    "LDFLAGS := -l$(EXEC)\n"
    "\n"
    "ifeq ($(CC), clang-18)\n"
    "\tLINTER := clang-tidy-18\n"
    "\tFORMATTER := clang-format-18\n"
    "\tCLANG_PREPARE := format lint\n"
    "endif\n"
    "\n"
    "# Optimization Flags\n"
    "ifeq ($(debug), 1)\n"
    "\tCFLAGS := $(CFLAGS) -g -O0\n"
    "else\n"
    "\tCFLAGS := $(CFLAGS) -g -Os\n"
    "endif\n"
    "\n"
    "# Entry\n"
    "all : $(EXEC)\n"
    "\n"
    "# Install executable for whole system\n"
    "install : $(CLANG_PREPARE) dir link\n"
    "\tcp $(abspath $(LIB_DIR)/$(SO_FILE)) /usr/local/lib\n"
    "\tchmod 0755 /usr/local/lib/$(SO_FILE)\n"
    "\tldconfig\n"
    "\t$(CC) $(CFLAGS) -o $(BIN_DIR)/$(EXEC) $(SRC_DIR)/main.c $(LDFLAGS) "
    "-I$(INCLUDE_DIR)\n"
    "\tcp $(abspath $(BIN_DIR)/$(EXEC)) /usr/local/bin\n"
    "\t@echo \"\\n\\033[32mDone.\\033[0m\"\n"
    "\n"
    "uninstall :\n"
    "\trm -rf /usr/local/lib/$(SO_FILE) /usr/local/bin/$(EXEC)\n"
    "\tldconfig\n"
    "\t@echo \"\\n\\033[32m$(EXEC) uninstalled.\\033[0m\"\n"
    "\n"
    "$(EXEC) : $(CLANG_PREPARE) dir link\n"
    "\t$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $(SRC_DIR)/main.c -L$(abspath "
    "$(LIB_DIR)) $(LDFLAGS) -I$(INCLUDE_DIR)\n"
    "\t@echo \"\\n\\033[33mRun command:\\n\\texport LD_LIBRARY_PATH=$(abspath "
    "$(LIB_DIR)):\"'$$LD_LIBRARY_PATH'\n"
    "\t@echo \"\\nOr add it to your .bashrc or .zshrc to make it available on "
    "bootup.\\033[0m\"\n"
    "\t@echo \"\\n\\033[32mDone.\\033[0m\"\n"
    "\n"
    "$(OBJS) : dir\n"
    "\t$(CC) $(CFLAGS) -fpic -o \"$(BUILD_DIR)/$(@F)\" -c \"$(*).c\" "
    "-I$(INCLUDE_DIR)\n"
    "\n"
    "link : dir $(OBJS)\n"
    "\t@if [ -z \"$(OBJS)\" ]; then echo $(ERROR_MESSAGE) ; exit 1; fi\n"
    "\t$(CC) -shared -o $(LIB_DIR)/$(SO_FILE) $(BUILD_DIR)/*\n"
    "\n"
    "format :\n"
    "\t@if [ -n \"$(HEADERS)\" ]; then $(FORMATTER) -style=file -i $(SRC_DIR)/* $(INCLUDE_DIR)/*; fi\n"
    "\n"
    "lint :\n"
    "\t@if [ -n \"$(HEADERS)\" ]; then $(LINTER) $(SRC_DIR)/*.c -- -I$(INCLUDE_DIR) $(CFLAGS); fi\n"
    "\n"
    "dir :\n"
    "\tmkdir -p $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR)\n"
    "\n"
    "clean :\n"
    "\trm -rf $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR)\n"
    "\n";

const char *clang_format_config =
"---\n"
"Language:        Cpp\n"
"BasedOnStyle:  Google\n"
"#AccessModifierOffset: -1\n"
"#AlignAfterOpenBracket: Align\n"
"#AlignConsecutiveAssignments: false\n"
"#AlignConsecutiveDeclarations: false\n"
"#AlignEscapedNewlinesLeft: true\n"
"#AlignOperands:   true\n"
"#AlignTrailingComments: true\n"
"#AllowAllParametersOfDeclarationOnNextLine: true\n"
"AllowShortBlocksOnASingleLine: true\n"
"AllowShortCaseLabelsOnASingleLine: true\n"
"#AllowShortFunctionsOnASingleLine: All\n"
"#AllowShortIfStatementsOnASingleLine: true\n"
"#AllowShortLoopsOnASingleLine: true\n"
"#AlwaysBreakAfterDefinitionReturnType: None\n"
"#AlwaysBreakAfterReturnType: None\n"
"AlwaysBreakBeforeMultilineStrings: false\n"
"#AlwaysBreakTemplateDeclarations: true\n"
"#BinPackArguments: true\n"
"#BinPackParameters: true\n"
"BreakBeforeBraces: Custom\n"
"BraceWrapping:   \n"
"  AfterClass:      true\n"
"#  AfterControlStatement: false\n"
"  AfterEnum:       true\n"
"  AfterFunction:   true\n"
"  AfterNamespace:  true\n"
"  AfterObjCDeclaration: true\n"
"  AfterStruct:     true\n"
"  AfterUnion:      true\n"
"#  BeforeCatch:     false\n"
"#  BeforeElse:      false\n"
"#  IndentBraces:    false\n"
"BreakBeforeBinaryOperators: NonAssignment\n"
"#BreakBeforeTernaryOperators: true\n"
"#BreakConstructorInitializersBeforeComma: false\n"
"#BreakAfterJavaFieldAnnotations: false\n"
"BreakStringLiterals: false\n"
"ColumnLimit:     88\n"
"CommentPragmas:  '^ Keep Comment:'\n"
"#ConstructorInitializerAllOnOneLineOrOnePerLine: true\n"
"#ConstructorInitializerIndentWidth: 4\n"
"#ContinuationIndentWidth: 4\n"
"#Cpp11BracedListStyle: true\n"
"#DerivePointerAlignment: true\n"
"#DisableFormat:   false\n"
"#ExperimentalAutoDetectBinPacking: false\n"
"#ForEachMacros:   [ foreach, Q_FOREACH, BOOST_FOREACH ]\n"
"#IncludeCategories: \n"
"#  - Regex:           '^<.*\\.h>'\n"
"#    Priority:        1\n"
"#  - Regex:           '^<.*'\n"
"#    Priority:        2\n"
"#  - Regex:           '.*'\n"
"#    Priority:        3\n"
"#IncludeIsMainRegex: '([-_](test|unittest))?$'\n"
"IncludeIsMainRegex: '$'\n"
"IndentCaseLabels: false\n"
"IndentWidth:     4\n"
"#IndentWrappedFunctionNames: false\n"
"#JavaScriptQuotes: Leave\n"
"#JavaScriptWrapImports: true\n"
"#KeepEmptyLinesAtTheStartOfBlocks: false\n"
"#MacroBlockBegin: ''\n"
"#MacroBlockEnd:   ''\n"
"#MaxEmptyLinesToKeep: 1\n"
"#NamespaceIndentation: None\n"
"#ObjCBlockIndentWidth: 2\n"
"#ObjCSpaceAfterProperty: false\n"
"#ObjCSpaceBeforeProtocolList: false\n"
"#PenaltyBreakBeforeFirstCallParameter: 1\n"
"#PenaltyBreakComment: 300\n"
"#PenaltyBreakFirstLessLess: 120\n"
"#PenaltyBreakString: 1000\n"
"#PenaltyExcessCharacter: 1000000\n"
"#PenaltyReturnTypeOnItsOwnLine: 200\n"
"PointerAlignment: Right\n"
"#ReflowComments:  true\n"
"#SortIncludes:    true\n"
"#SpaceAfterCStyleCast: false\n"
"#SpaceAfterTemplateKeyword: true\n"
"#SpaceBeforeAssignmentOperators: true\n"
"#SpaceBeforeParens: ControlStatements\n"
"#SpaceInEmptyParentheses: false\n"
"#SpacesBeforeTrailingComments: 2\n"
"#SpacesInAngles:  false\n"
"SpacesInContainerLiterals: false\n"
"#SpacesInCStyleCastParentheses: false\n"
"#SpacesInParentheses: false\n"
"#SpacesInSquareBrackets: false\n"
"#Standard:        Auto\n"
"TabWidth:        4\n"
"UseTab:          Never\n"
"...\n";

const char *clangd_config =
    "CompileFlags:\n"
    "  Add: [-I/home/ervinpicardal/cpp_ws/sercomm_training/PRACTICE/ccp/include]";

const char *clang_tidy_config =
    "---\n"
    "Checks:          "
    "'clang-diagnostic-*,clang-analyzer-*,-clang-analyzer-security.insecureAPI."
    "DeprecatedOrUnsafeBufferHandling'\n"
    "WarningsAsErrors: ''\n"
    "HeaderFileExtensions:\n"
    "  - ''\n"
    "  - h\n"
    "  - hh\n"
    "  - hpp\n"
    "  - hxx\n"
    "ImplementationFileExtensions:\n"
    "  - c\n"
    "  - cc\n"
    "  - cpp\n"
    "  - cxx\n"
    "HeaderFilterRegex: ''\n"
    "FormatStyle:     none\n"
    "User:            ervinpicardal\n"
    "CheckOptions:\n"
    "  cert-dcl16-c.NewSuffixes: 'L;LL;LU;LLU'\n"
    "  cert-err33-c.AllowCastToVoid: 'true'\n"
    "  cert-err33-c.CheckedFunctions: "
    "'::aligned_alloc;::asctime_s;::at_quick_exit;::atexit;::bsearch;::bsearch_"
    "s;::btowc;::c16rtomb;::c32rtomb;::calloc;::clock;::cnd_broadcast;::cnd_"
    "init;::cnd_signal;::cnd_timedwait;::cnd_wait;::ctime_s;::fclose;::fflush;:"
    ":fgetc;::fgetpos;::fgets;::fgetwc;::fopen;::fopen_s;::fprintf;::fprintf_s;"
    "::fputc;::fputs;::fputwc;::fputws;::fread;::freopen;::freopen_s;::fscanf;:"
    ":fscanf_s;::fseek;::fsetpos;::ftell;::fwprintf;::fwprintf_s;::fwrite;::"
    "fwscanf;::fwscanf_s;::getc;::getchar;::getenv;::getenv_s;::gets_s;::getwc;"
    "::getwchar;::gmtime;::gmtime_s;::localtime;::localtime_s;::malloc;::"
    "mbrtoc16;::mbrtoc32;::mbsrtowcs;::mbsrtowcs_s;::mbstowcs;::mbstowcs_s;::"
    "memchr;::mktime;::mtx_init;::mtx_lock;::mtx_timedlock;::mtx_trylock;::mtx_"
    "unlock;::printf_s;::putc;::putwc;::raise;::realloc;::remove;::rename;::"
    "scanf;::scanf_s;::setlocale;::setvbuf;::signal;::snprintf;::snprintf_s;::"
    "sprintf;::sprintf_s;::sscanf;::sscanf_s;::strchr;::strerror_s;::strftime;:"
    ":strpbrk;::strrchr;::strstr;::strtod;::strtof;::strtoimax;::strtok;::"
    "strtok_s;::strtol;::strtold;::strtoll;::strtoul;::strtoull;::strtoumax;::"
    "strxfrm;::swprintf;::swprintf_s;::swscanf;::swscanf_s;::thrd_create;::"
    "thrd_detach;::thrd_join;::thrd_sleep;::time;::timespec_get;::tmpfile;::"
    "tmpfile_s;::tmpnam;::tmpnam_s;::tss_create;::tss_get;::tss_set;::ungetc;::"
    "ungetwc;::vfprintf;::vfprintf_s;::vfscanf;::vfscanf_s;::vfwprintf;::"
    "vfwprintf_s;::vfwscanf;::vfwscanf_s;::vprintf_s;::vscanf;::vscanf_s;::"
    "vsnprintf;::vsnprintf_s;::vsprintf;::vsprintf_s;::vsscanf;::vsscanf_s;::"
    "vswprintf;::vswprintf_s;::vswscanf;::vswscanf_s;::vwprintf_s;::vwscanf;::"
    "vwscanf_s;::wcrtomb;::wcschr;::wcsftime;::wcspbrk;::wcsrchr;::wcsrtombs;::"
    "wcsrtombs_s;::wcsstr;::wcstod;::wcstof;::wcstoimax;::wcstok;::wcstok_s;::"
    "wcstol;::wcstold;::wcstoll;::wcstombs;::wcstombs_s;::wcstoul;::wcstoull;::"
    "wcstoumax;::wcsxfrm;::wctob;::wctrans;::wctype;::wmemchr;::wprintf_s;::"
    "wscanf;::wscanf_s;'\n"
    "  cert-oop54-cpp.WarnOnlyIfThisHasSuspiciousField: 'false'\n"
    "  cert-str34-c.DiagnoseSignedUnsignedCharComparisons: 'false'\n"
    "  "
    "cppcoreguidelines-non-private-member-variables-in-classes."
    "IgnoreClassesWithAllMemberVariablesBeingPublic: 'true'\n"
    "  google-readability-braces-around-statements.ShortStatementLines: '1'\n"
    "  google-readability-function-size.StatementThreshold: '800'\n"
    "  google-readability-namespace-comments.ShortNamespaceLines: '10'\n"
    "  google-readability-namespace-comments.SpacesBeforeComments: '2'\n"
    "  llvm-else-after-return.WarnOnConditionVariables: 'false'\n"
    "  llvm-else-after-return.WarnOnUnfixable: 'false'\n"
    "  llvm-qualified-auto.AddConstToQualified: 'false'\n"
    "SystemHeaders:   false\n"
    "...\n";

const char *main_file =
    "#include <stdio.h>\n"
    "#include <stdlib.h>\n"
    "#include <string.h>\n"
    "\n"
    "int main(void)\n"
    "{\n"
    "\tprintf(\"Hello World!\\n\");\n"
    "\n"
    "\treturn 0;\n"
    "}";

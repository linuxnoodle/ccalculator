#include "eval.h"

// technically libm is a seperate thing so i'll implement all math functions myself
// TODO: move to CORE-MATH implementations (when i can understand them)
// https://core-math.gitlabpages.inria.fr/

/* ================== BEGIN HELPER FUNCTIONS ================== */
double floor(double input){
    int i = (int)input;
    if (i > input)
        return i - 1;
    return i;
}

double mod_f(double a, double b){
    // a % b = a - b * floor(a / b)
    return a - b * floor(a / b);
}
/* ================== END HELPER FUNCTIONS ================== */

/* ================== BEGIN POWER FUNCTIONS ================== */
double exp_f(double exponent){
    // compute e^x as a taylor series
    double sum = 1;
    double term = 1;
    for (int i = 1; i < exponent + 100; i++){
        term *= exponent / i;
        sum += term;
    }
    return sum;
}

double log_f(double input){
    // taylor series for log(1 + x) 
    double a = 1;
    double y = (input - a) / (input + a);
    double sum = 2 * y;
    double term = 2 * y;
    // convergence is rather slow, 
    // so terms are in relation to the input size
    // TODO: find more efficient alg
    for (int i = 3; i < input * 2 + 100; i += 2){
        term *= y * y;
        sum += term / i;
    }
    return sum;
}
double pow_f(double base, double exponent){
    // use the identity a^b = e^(b * ln(a))
    return exp_f(exponent * log_f(base));
}
/* ================== END POWER FUNCTIONS ================== */

/* ================== BEGIN TRIG FUNCTIONS ================== */
// minimax approximations from https://publik-void.github.io/sin-cos-approximations/
double sin_f(double input){
    double x1 = mod_f(input, 2 * PI);
    double x2 = x1 * x1;
    return x1 * (0.99999999999999903148008418378595416 +
           x2 * (-0.166666666666647809166043102595249597 +
           x2 * (0.00833333333322623603809875967403959193 +
           x2 * (-0.000198412698139567192404904096148800949 +
           x2 * (2.75573155289183671003278704218629314e-6 +
           x2 * (-2.50518246481347370240543449781033653e-8 +
           x2 * (1.60466203872904879366205078294157963e-10 -
           7.35766011971809330900478644486164214e-13 * x2)))))));
}

// seperate minimax function for more precision
double cos_f(double input){
    double x1 = mod_f(input, 2 * PI); // domain of approximation is [-pi, pi]
    double x2 = x1 * x1;
    return 0.99999999999999999608981951072301546 +
           x2 * (-0.49999999999999974308667070690271678 +
           x2 * (0.041666666666663887964497659102665977 +
           x2 * (-0.00138888888887731722447063574018337351 +
           x2 * (0.0000248015872774439536215582612189964674 +
           x2 * (-2.75573163935355075808996593111486352e-7 +
           x2 * (2.08765619601383980877940138085106396e-9 +
           x2 * (-1.14629048996344469555493302470243688e-11 +
           4.60900737685258733987906059894416831e-14 * x2)))))));
}

double tan_f(double input){
    return sin_f(input) / cos_f(input);
}
/* ================== END TRIG FUNCTIONS ================== */

double lookup(TEXT_ENUM type, Node** params, size_t param_count){
    // get first parameter
    if (!params){
        fprintf(stderr, "\nERROR: No parameters provided to function.\n");
        return 0;
    }
    double *evaluated_params = malloc(sizeof(double) * param_count);
    for (int i = 0; i < param_count; i++){
        evaluated_params[i] = evaluate_f(params[i]);
    }
    switch (type){ // technically needs to free params but whatever
        // functions
        case SIN:
            return sin_f(evaluated_params[0]);
        case COS: 
            return cos_f(evaluated_params[0]);
        case TAN:
            return tan_f(evaluated_params[0]);
        case LN:
            return log_f(evaluated_params[0]);
        case EXP:
            return exp_f(evaluated_params[0]);
        case SQRT:
            return pow_f(evaluated_params[0], 0.5);
        // constants
        default:
            fprintf(stderr, "ERROR: Invalid name\n");
            return 0; 
    }
}

double evaluate_f(Node *tree){
    double left = 0, right = 0;
    if (tree && !tree->left && !tree->right){
        if (tree->t->type != TOKEN_TEXT)
            return atof(tree->t->contents);
    } else if (tree && tree->left && tree->right){
        left = evaluate_f(tree->left);
        right = evaluate_f(tree->right);
    } else if (!tree){
        fprintf(stderr, "ERROR: Invalid syntax tree\n");
        return 0;
    }

    switch (tree->t->type){
        case TOKEN_PLUS:
            return left + right;
        case TOKEN_MINUS:
            return left - right;
        case TOKEN_MULTIPLY:
            return left * right;
        case TOKEN_DIVIDE:
            return left / right;
        case TOKEN_POW:
            return pow_f(left, right);
        case TOKEN_TEXT:
            return lookup(tree->t->func.text,
                          (Node**)tree->t->func.parameters,
                          tree->t->func.param_count);
        default:
            fprintf(stderr, "ERROR: Invalid token type\n");
            return 0;
    }
}

bool is_numeric(char *str){
    for (int i = 0; i < strlen(str); i++){
        if (!isdigit(str[i]) && str[i] != '.'){
            return false;
        }
    }
    return true;
}

// TODO
char *get_analytic_result(Node *n){
    if (n->t->type != TOKEN_TEXT){
        return "how did you even"; // should never be ran
    }
    switch (n->t->func.text){
        // check if exact multiple of pi
        case SIN:
        case COS:
        case TAN:
            break;
        case SQRT:
            break;
        default:
            break;
    }
}

char *evaluate_exact(Node *tree){
    if (!tree->left && !tree->right){
        return tree->t->contents;
    }
    // if the operation isn't a function, evaluate 
    if (tree->t->type != TOKEN_TEXT){
        char *left = evaluate_exact(tree->left);
        char *right = evaluate_exact(tree->right);
        char *str = malloc(sizeof(char) * 100);
        sprintf(str, "(%s %s %s)", left, tree->t->contents, right);
        return str;
    } else {
        return get_analytic_result(tree);
    }
    char *left = evaluate_exact(tree->left);
    char *right = evaluate_exact(tree->right);
    char *str = malloc(sizeof(char) * 100);
    sprintf(str, "%s%s", left, right);
    return str;
}

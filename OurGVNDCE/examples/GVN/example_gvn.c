int gvn_local_transitive(int a, int b) {
    int first = a + b;
    int second = b + a;
    int left = first * 2;
    int right = second * 2;
    return left + right;
}

int gvn_dominating_branch(int condition, int a, int b) {
    int first = a + b;

    if (condition) {
      int second = a + b;
      return second * 2;
    }

    int third = a + b;
    return third - 1;
}

int gvn_sibling_branches(int condition, int a, int b) {
    if (condition) {
      return a + b;
    }

    return a + b;
}

int gvn_comparisons(int a, int b) {
    int equal_first = a == b;
    int equal_second = b == a;
    int less_forward = a < b;
    int less_reverse = b < a;
    return (equal_first ^ equal_second) ^ (less_forward ^ less_reverse);
}

int gvn_division(int a, int b) {
      int first = a / b;
      int same = a / b;
      unsigned int unsigned_value = (unsigned int) a / (unsigned int) b;
      int reversed = b / a;
      return first + same + (int) unsigned_value + reversed;
}

int gvn_memory_conservative(int *value) {
      int first = *value + 1;
      int second = *value + 1;
      return first + second;
}

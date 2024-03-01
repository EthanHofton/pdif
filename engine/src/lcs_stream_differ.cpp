#include <pdif/lcs_stream_differ.hpp>

namespace pdif {

void lcs_stream_differ::diff(pdif::diff& diff) {
    // Step 1: Generate the LCS matrix
    int m = stream1.size();
    int n = stream2.size();
    int i, j;

    std::vector<std::vector<int>> L(m + 1, std::vector<int>(n + 1, 0));
    std::vector<std::vector<int>> D(m + 1, std::vector<int>(n + 1, 0));

    for (j = 1; j < n + 1; j++) {
        for (i = 1; i < m + 1; i++) {
            if (stream1[i-1]->compare(stream2[j-1])) {
                L[i][j] = L[j-1][i-1] + 1;
                D[i][j] = 0;
            } else {
                if (L[j-1][i] > L[j][i-1]) {
                    L[j][i] = L[j-1][i];
                    D[j][i] = 1;
                } else {
                    L[j][i] = L[j][i-1];
                    D[j][i] = -1;
                }
            }
        }
    }

    // Step 2: backtrace to find the LCS
    i = m;
    j = n;

    while (i > 0 && j > 0) {
        if (D[j][i] == 0) {
            diff.add_edit_op(edit_op(edit_op_type::EQ));
            i--;
            j--;
        } else if (D[j][i] == 1) {
            diff.add_edit_op(edit_op(edit_op_type::INSERT, stream2[j-1]));
            j--;
        } else {
            diff.add_edit_op(edit_op(edit_op_type::DELETE));
            i--;
        }
    }

    while (i > 0) {
        diff.add_edit_op(edit_op(edit_op_type::DELETE));
        i--;
    }

    while (j > 0) {
        diff.add_edit_op(edit_op(edit_op_type::INSERT, stream2[j-1]));
        j--;
    }

    diff.reverse_edit_ops();
}

}
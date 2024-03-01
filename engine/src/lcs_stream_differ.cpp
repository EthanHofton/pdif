#include <pdif/lcs_stream_differ.hpp>

namespace pdif {

void lcs_stream_differ::diff(pdif::diff& diff) {
    // Step 1: Generate the LCS matrix
    int m = stream1.size();
    int n = stream2.size();
    int i, j;

    std::vector<std::vector<int>> L;
    // std::vector<std::vector<std::string>> D;
    std::vector<std::vector<int>> D;

    for (int j = 0; j <= n; ++j) {
        L.push_back(std::vector<int>());
        // D.push_back(std::vector<std::string>());
        D.push_back(std::vector<int>());
        for (int i = 0; i <= m; ++i) {
            L[j].push_back(0);
            // D[j].push_back("");
            D[j].push_back(0);
        }
    }

    for (int j = 1; j <= n; ++j) {
        for (int i = 1; i <= m; ++i) {
            if (stream1[i-1]->compare(stream2[j-1])) {
                L[j][i] = L[j-1][i-1] + 1;
                // D[j][i] = "\u2196"; // "diag"
                D[j][i] = 0;
            } else {
                if (L[j-1][i] > L[j][i-1]) {
                    L[j][i] = L[j-1][i];
                    // D[j][i] = "\u2191"; // "up"
                    D[j][i] = 1;
                } else {
                    L[j][i] = L[j][i-1];
                    // D[j][i] = "\u2190"; // "left"
                    D[j][i] = 2;
                }
            }
        }
    }

    // Step 2: backtrace to find the LCS
    i = m;
    j = n;

    while (i > 0 && j > 0) {
        // if (D[j][i] == "\u2196") {
        if (D[j][i] == 0) {
            diff.add_edit_op(edit_op(edit_op_type::EQ));
            --i;
            --j;
        // } else if (D[j][i] == "\u2191") {
        } else if (D[j][i] == 1) {
            diff.add_edit_op(edit_op(edit_op_type::INSERT, stream2[j-1]));
            --j;
        // } else if (D[j][i] == "\u2190") {
        } else if (D[j][i] == 2) {
            diff.add_edit_op(edit_op(edit_op_type::DELETE));
            --i;
        }
    }

    while (i > 0) {
        diff.add_edit_op(edit_op(edit_op_type::DELETE));
        --i;
    }

    while (j > 0) {
        diff.add_edit_op(edit_op(edit_op_type::INSERT, stream2[j-1]));
        --j;
    }

    diff.reverse_edit_ops();
}

}

typedef class vsc_expr_bit;
class vsc_constraint_set extends vsc_expr_bit #(.W(1));
    rand vsc_expr_bit #(1)  constraints[$];
    rand bit                valid[$];

    constraint valid_c {
        foreach (constraints[i]) {
            valid[i] == constraints[i].value;
        }
        valid.and() == 1;
    }

    virtual function void add_constraint(vsc_expr_bit #(1) expr);
        constraints.push_back(expr);
        valid.push_back(1);
    endfunction

endclass
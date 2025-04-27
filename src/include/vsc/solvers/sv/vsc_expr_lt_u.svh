
typedef class vsc_expr_bit;
class vsc_expr_lt_u #(int W) extends vsc_expr_bit #(.W(1));
    rand vsc_expr_bit #(W) lhs;
    rand vsc_expr_bit #(W) rhs;

    constraint lt_c {
        value == (lhs.value < rhs.value);
    }

    function new(vsc_expr_bit #(W) lhs, vsc_expr_bit #(W) rhs);
        this.lhs = lhs;
        this.rhs = rhs;
    endfunction

endclass

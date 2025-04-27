typedef class vsc_expr_bit;
class vsc_expr_zext #(int W, int I) extends vsc_expr_bit #(W);
    rand vsc_expr_bit #(I) rhs;

    constraint zext_c {
        value == rhs.value;
    }

    function new(vsc_expr_bit #(I) rhs);
        this.rhs = rhs;
    endfunction

endclass

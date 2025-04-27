typedef class vsc_expr_bit;
class vsc_if_else extends vsc_expr_bit #(1);
    rand vsc_expr_bit #(1) cond;
    rand vsc_expr_bit #(1) true_expr;
    rand vsc_expr_bit #(1) false_expr;

    constraint if_else_c {
        value == (cond.value ? true_expr.value : false_expr.value);
    }

    function new(
        vsc_expr_bit #(1) cond,
        vsc_expr_bit #(1) true_expr, 
        vsc_expr_bit #(1) false_expr);
        this.cond = cond;
        this.true_expr = true_expr;
        this.false_expr = false_expr;
    endfunction

endclass
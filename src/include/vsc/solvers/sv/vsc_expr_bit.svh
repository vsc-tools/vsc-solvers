
typedef class vsc_expr;
typedef class vsc_expr_zext;
class vsc_expr_bit #(int W) extends vsc_expr;
    rand bit[W-1:0] value;

    virtual function int width();
        return -1;
    endfunction

    virtual function bit is_signed();
        return 0;
    endfunction

    virtual function vsc_expr zero_extend(int w);
        vsc_expr ret;
        if (w <= 8) begin
            vsc_expr_zext #(.W(8), .I(W)) expr = new(this);
            ret = expr;
        end else if (w <= 16) begin
            vsc_expr_zext #(.W(16), .I(W)) expr = new(this);
            ret = expr;
        end else if (w <= 32) begin
            vsc_expr_zext #(.W(32), .I(W)) expr = new(this);
            ret = expr;
        end else if (w <= 64) begin
            vsc_expr_zext #(.W(64), .I(W)) expr = new(this);
            ret = expr;
        end else if (w <= 128) begin
            vsc_expr_zext #(.W(128), .I(W)) expr = new(this);
            ret = expr;
        end else if (w <= 256) begin
            vsc_expr_zext #(.W(256), .I(W)) expr = new(this);
            ret = expr;
        end else if (w <= 512) begin
            vsc_expr_zext #(.W(512), .I(W)) expr = new(this);
            ret = expr;
        end else if (w <= 1024) begin
            vsc_expr_zext #(.W(1024), .I(W)) expr = new(this);
            ret = expr;
        end
        return ret;
    endfunction

    virtual function vsc_expr sign_extend(int w);
        return null;
    endfunction

endclass
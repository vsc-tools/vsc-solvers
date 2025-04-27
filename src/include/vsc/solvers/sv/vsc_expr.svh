
class vsc_expr;
    virtual function int width();
        return -1;
    endfunction

    virtual function bit is_signed();
        return 0;
    endfunction

    virtual function vsc_expr zero_extend(int w);
        return null;
    endfunction

    virtual function vsc_expr sign_extend(int w);
        return null;
    endfunction

endclass

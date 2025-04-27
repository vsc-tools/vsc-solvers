
module top;
  import vsc_solvers::*;

initial begin
  automatic vsc_expr_bit #(8) a = new();
  automatic vsc_expr_bit #(8) b = new();
  automatic vsc_expr_bit #(1) c = new();
  automatic vsc_expr_lt_u #(8) lt_1 = new(a, b);
  automatic vsc_expr_lt_u #(8) lt_2 = new(b, a);
  automatic vsc_if_else if_else = new(c, lt_1, lt_2);
  automatic vsc_constraint_set obj = new();

  obj.add_constraint(if_else);

  repeat (1000000) begin
  if (obj.randomize() == 0) begin
    $display("Failed to solve");
  end else begin
//    $display("Solved: %0d %0d %0d", c.value, a.value, b.value);
  end
  end

  $finish;


end

endmodule



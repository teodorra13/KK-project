
define dso_local i32 @gvn_local_transitive(i32 noundef %0, i32 noundef %1) #0 {
  %3 = add i32 %0, %1
  %4 = add i32 %1, %0
  %5 = mul i32 %3, 2
  %6 = mul i32 %4, 2
  %7 = add i32 %5, %6
  ret i32 %7
}

define dso_local i32 @gvn_dominating_branch(i32 noundef %0, i32 noundef %1, i32 noundef %2) #0 {
  %4 = add i32 %1, %2
  %5 = icmp ne i32 %0, 0
  br i1 %5, label %6, label %9

6:                                                ; preds = %3
  %7 = add i32 %1, %2
  %8 = mul i32 %7, 2
  br label %12

9:                                                ; preds = %3
  %10 = add i32 %1, %2
  %11 = sub i32 %10, 1
  br label %12

12:                                               ; preds = %9, %6
  %.0 = phi i32 [ %8, %6 ], [ %11, %9 ]
  ret i32 %.0
}


define dso_local i32 @gvn_sibling_branches(i32 noundef %0, i32 noundef %1, i32 noundef %2) #0 {
  %4 = icmp ne i32 %0, 0
  br i1 %4, label %5, label %7

5:                                                ; preds = %3
  %6 = add i32 %1, %2
  br label %9

7:                                                ; preds = %3
  %8 = add i32 %1, %2
  br label %9

9:                                                ; preds = %7, %5
  %.0 = phi i32 [ %6, %5 ], [ %8, %7 ]
  ret i32 %.0
}

define dso_local i32 @gvn_comparisons(i32 noundef %0, i32 noundef %1) #0 {
  %3 = icmp eq i32 %0, %1
  %4 = zext i1 %3 to i32
  %5 = icmp eq i32 %1, %0
  %6 = zext i1 %5 to i32
  %7 = icmp slt i32 %0, %1
  %8 = zext i1 %7 to i32
  %9 = icmp slt i32 %1, %0
  %10 = zext i1 %9 to i32
  %11 = xor i32 %4, %6
  %12 = xor i32 %8, %10
  %13 = xor i32 %11, %12
  ret i32 %13
}

define dso_local i32 @gvn_division(i32 noundef %0, i32 noundef %1) #0 {
  %3 = sdiv i32 %0, %1
  %4 = sdiv i32 %0, %1
  %5 = udiv i32 %0, %1
  %6 = sdiv i32 %1, %0
  %7 = add i32 %3, %4
  %8 = add i32 %7, %5
  %9 = add i32 %8, %6
  ret i32 %9
}


define dso_local i32 @gvn_memory_conservative(ptr noundef %0) #0 {
  %2 = load i32, ptr %0, align 4
  %3 = add i32 %2, 1
  %4 = load i32, ptr %0, align 4
  %5 = add i32 %4, 1
  %6 = add i32 %3, %5
  ret i32 %6
}

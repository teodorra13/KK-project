

define dso_local i32 @gvn_local_transitive(i32 noundef %0, i32 noundef %1) #0 {
  %3 = add i32 %0, %1
  %4 = mul i32 %3, 2
  %5 = add i32 %4, %4
  ret i32 %5
}

define dso_local i32 @gvn_dominating_branch(i32 noundef %0, i32 noundef %1, i32 noundef %2) #0 {
  %4 = add i32 %1, %2
  %5 = icmp ne i32 %0, 0
  br i1 %5, label %6, label %8

6:                                                ; preds = %3
  %7 = mul i32 %4, 2
  br label %10

8:                                                ; preds = %3
  %9 = sub i32 %4, 1
  br label %10

10:                                               ; preds = %8, %6
  %.0 = phi i32 [ %7, %6 ], [ %9, %8 ]
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
  %5 = zext i1 %3 to i32
  %6 = icmp slt i32 %0, %1
  %7 = zext i1 %6 to i32
  %8 = icmp slt i32 %1, %0
  %9 = zext i1 %8 to i32
  %10 = xor i32 %4, %5
  %11 = xor i32 %7, %9
  %12 = xor i32 %10, %11
  ret i32 %12
}

define dso_local i32 @gvn_division(i32 noundef %0, i32 noundef %1) #0 {
  %3 = sdiv i32 %0, %1
  %4 = udiv i32 %0, %1
  %5 = sdiv i32 %1, %0
  %6 = add i32 %3, %3
  %7 = add i32 %6, %4
  %8 = add i32 %7, %5
  ret i32 %8
}

define dso_local i32 @gvn_memory_conservative(ptr noundef %0) #0 {
  %2 = load i32, ptr %0, align 4
  %3 = add i32 %2, 1
  %4 = load i32, ptr %0, align 4
  %5 = add i32 %4, 1
  %6 = add i32 %3, %5
  ret i32 %6
}

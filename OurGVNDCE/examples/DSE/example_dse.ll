; ModuleID = 'example_dse.c'
source_filename = "example_dse.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @dead_store_simple() #0 {
  %1 = alloca i32, align 4
  store i32 10, ptr %1, align 4
  ret i32 0
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @live_store() #0 {
  %1 = alloca i32, align 4
  store i32 10, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  ret i32 %2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @overwrite_store() #0 {
  %1 = alloca i32, align 4
  store i32 10, ptr %1, align 4
  store i32 20, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  ret i32 %2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @multiple_overwrite() #0 {
  %1 = alloca i32, align 4
  store i32 1, ptr %1, align 4
  store i32 2, ptr %1, align 4
  store i32 3, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  ret i32 %2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @store_before_call() #0 {
  %1 = alloca i32, align 4
  store i32 10, ptr %1, align 4
  call void (...) @unknown_function()
  ret i32 0
}

declare void @unknown_function(...) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @store_after_call() #0 {
  %1 = alloca i32, align 4
  call void (...) @unknown_function()
  store i32 10, ptr %1, align 4
  ret i32 0
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @overwrite_after_call() #0 {
  %1 = alloca i32, align 4
  store i32 10, ptr %1, align 4
  call void (...) @unknown_function()
  store i32 20, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  ret i32 %2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @two_store_after_call() #0 {
  %1 = alloca i32, align 4
  call void (...) @unknown_function()
  store i32 10, ptr %1, align 4
  store i32 20, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  ret i32 %2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @multiple_variables_after_call() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  call void (...) @unknown_function()
  store i32 10, ptr %1, align 4
  store i32 20, ptr %2, align 4
  store i32 30, ptr %1, align 4
  %3 = load i32, ptr %2, align 4
  ret i32 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @load_between_stores() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  call void (...) @unknown_function()
  store i32 10, ptr %1, align 4
  %3 = load i32, ptr %1, align 4
  store i32 %3, ptr %2, align 4
  store i32 20, ptr %1, align 4
  %4 = load i32, ptr %2, align 4
  ret i32 %4
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @multiple_calls() #0 {
  %1 = alloca i32, align 4
  store i32 10, ptr %1, align 4
  call void (...) @unknown_function()
  store i32 20, ptr %1, align 4
  call void (...) @unknown_function()
  store i32 30, ptr %1, align 4
  ret i32 0
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @declare_call_test() #0 {
  call void (...) @unknown_function()
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 17.0.0"}


define dso_local i32 @dead_store_simple() #0 {
  %1 = alloca i32, align 4
  store i32 10, ptr %1, align 4
  ret i32 0
}


define dso_local i32 @live_store() #0 {
  %1 = alloca i32, align 4
  store i32 10, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  ret i32 %2
}


define dso_local i32 @overwrite_store() #0 {
  %1 = alloca i32, align 4
  store i32 10, ptr %1, align 4
  store i32 20, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  ret i32 %2
}


define dso_local i32 @multiple_overwrite() #0 {
  %1 = alloca i32, align 4
  store i32 1, ptr %1, align 4
  store i32 2, ptr %1, align 4
  store i32 3, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  ret i32 %2
}

define dso_local i32 @store_before_call() #0 {
  %1 = alloca i32, align 4
  store i32 10, ptr %1, align 4
  call void (...) @unknown_function()
  ret i32 0
}

declare void @unknown_function(...) #1


define dso_local i32 @store_after_call() #0 {
  %1 = alloca i32, align 4
  call void (...) @unknown_function()
  store i32 10, ptr %1, align 4
  ret i32 0
}


define dso_local i32 @overwrite_after_call() #0 {
  %1 = alloca i32, align 4
  store i32 10, ptr %1, align 4
  call void (...) @unknown_function()
  store i32 20, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  ret i32 %2
}


define dso_local i32 @two_store_after_call() #0 {
  %1 = alloca i32, align 4
  call void (...) @unknown_function()
  store i32 10, ptr %1, align 4
  store i32 20, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  ret i32 %2
}

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


define dso_local i32 @multiple_calls() #0 {
  %1 = alloca i32, align 4
  store i32 10, ptr %1, align 4
  call void (...) @unknown_function()
  store i32 20, ptr %1, align 4
  call void (...) @unknown_function()
  store i32 30, ptr %1, align 4
  ret i32 0
}


define dso_local i32 @declare_call_test() #0 {
  call void (...) @unknown_function()
  ret i32 0
}



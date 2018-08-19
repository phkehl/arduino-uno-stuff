BEGIN { error = 0; }
/% Full/ {
    match($$0, /^(.+):.+\(([0-9]+\.[0-9]+)% Full\)/, a);
    if (a[2] > 100.0)
    {
        print("Too much " $0);
        error++;
    }
}
END {
    exit(error ? 1 : 0);
}

public class JNITestDerived extends JNITestBase {
    @Override public int virtualValue() { return 20; }
    @Override public String virtualString() { return "derived"; }
}

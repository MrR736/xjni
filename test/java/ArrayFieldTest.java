public class ArrayFieldTest {
    static { System.loadLibrary("xjni_test"); }

    // ===== instance fields =====
    public String[] stringArray;
    public int[] intArray;

    // ===== static fields =====
    public static String[] staticStringArray;
    public static int[] staticIntArray;

    // native test entry
    private native void nativeTest();

    public static void main(String[] args) {
        ArrayFieldTest t = new ArrayFieldTest();

        t.stringArray = new String[] { "A", "B", "C" };
        t.intArray = new int[] { 1, 2, 3 };

        staticStringArray = new String[] { "X", "Y", "Z" };
        staticIntArray = new int[] { 10, 20, 30 };

        t.nativeTest();

        System.out.println("After native call:");
        for (String s : t.stringArray)
            System.out.println("stringArray: " + s);

        for (int i : t.intArray)
            System.out.println("intArray: " + i);

        for (String s : staticStringArray)
            System.out.println("staticStringArray: " + s);

        for (int i : staticIntArray)
            System.out.println("staticIntArray: " + i);
    }
}

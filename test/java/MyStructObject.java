public class MyStructObject {
    public int field;
    public double value;

    static { System.loadLibrary("xjni_test"); }

    public static native MyStructObject create();
    public static native void modify(MyStructObject obj);

    public static void main(String[] args) {
        MyStructObject obj = MyStructObject.create();

        System.out.println(obj.field + " " + obj.value);

        MyStructObject.modify(obj);

        System.out.println(obj.field + " " + obj.value);
    }
}

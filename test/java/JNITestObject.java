public class JNITestObject {
    public int intField = 11;
    public long longField = 22L;
    public float floatField = 1.5f;
    public double doubleField = 2.5;
    public byte byteField = 3;
    public short shortField = 4;
    public char charField = 'A';
    public boolean booleanField = true;
    public String objectField = "field";

    public static int staticIntField = 101;
    public static long staticLongField = 102L;
    public static float staticFloatField = 103.5f;
    public static double staticDoubleField = 104.5;
    public static byte staticByteField = 105;
    public static short staticShortField = 106;
    public static char staticCharField = 'S';
    public static boolean staticBooleanField = true;
    public static String staticObjectField = "static";

    public JNITestObject() {}
    public JNITestObject(int v) { intField = v; }

    public int add(int a, int b) { return a + b; }
    public long addLong(long a, long b) { return a + b; }
    public float addFloat(float a, float b) { return a + b; }
    public double addDouble(double a, double b) { return a + b; }
    public byte addByte(byte a, byte b) { return (byte)(a + b); }
    public short addShort(short a, short b) { return (short)(a + b); }
    public char nextChar(char c) { return (char)(c + 1); }
    public boolean and(boolean a, boolean b) { return a && b; }
    public String concat(String a, String b) { return a + b; }
    public Object self() { return this; }
    public void setInt(int v) { intField = v; }

    public static int staticAdd(int a, int b) { return a + b; }
    public static long staticLong(long a, long b) { return a + b; }
    public static float staticFloat(float a, float b) { return a + b; }
    public static double staticDouble(double a, double b) { return a + b; }
    public static byte staticByte(byte a, byte b) { return (byte)(a + b); }
    public static short staticShort(short a, short b) { return (short)(a + b); }
    public static char staticChar(char a) { return (char)(a + 2); }
    public static boolean staticAnd(boolean a, boolean b) { return a && b; }
    public static String staticConcat(String a, String b) { return a + b; }
    public static Object staticSelf(JNITestObject o) { return o; }
    public static void staticSetInt(JNITestObject o, int v) { o.intField = v; }

    public String instanceOnly() { return "instance"; }
}

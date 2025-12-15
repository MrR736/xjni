public class Array2DTest {
    static { System.loadLibrary("xjni_test"); }
    public int[][] int2d;
    public String[][] str2d;

    private native void nativeTest();

    public static void main(String[] args) {
        Array2DTest t = new Array2DTest();

        t.int2d = new int[][] {
            {1, 2, 3},
            {4, 5, 6}
        };

        t.str2d = new String[][] {
            {"A", "B"},
            {"C", "D"}
        };

        t.nativeTest();

        System.out.println("After native call:");

        for (int i = 0; i < t.int2d.length; i++)
            for (int j = 0; j < t.int2d[i].length; j++)
                System.out.println("int2d[" + i + "][" + j + "] = " + t.int2d[i][j]);

        for (int i = 0; i < t.str2d.length; i++)
            for (int j = 0; j < t.str2d[i].length; j++)
                System.out.println("str2d[" + i + "][" + j + "] = " + t.str2d[i][j]);
    }
}

package imgproc;

public abstract class HomomorphicFilter
{
	public static native void homomorphicFilter(byte[] pFrame, int pWidth, int pHeight);
}
// read tutorial here:
// http://catlikecoding.com/unity/tutorials/procedural-grid/

using System.Collections;
using UnityEngine;

// Grid script requires the GameObject to have a MeshFilter
// and MeshRenderer component
[RequireComponent(typeof(MeshFilter), typeof(MeshRenderer))]
public class Grid2 : MonoBehaviour {

    public int xSize, ySize;
    private Vector3[] vertices;
    private Mesh mesh;


    // when we enter play mode
    private void Awake()
    {
        StartCoroutine(Generate());
    }

    private void OnDrawGizmos()
    {
        if (vertices == null) return;
        Gizmos.color = Color.black;
        for(int i = 0; i < vertices.Length; i++)
        {
            Gizmos.DrawSphere(vertices[i], 0.2f);
        }
    }

    // Use this for initialization
    void Start ()
    {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    private IEnumerator Generate()
    {
        WaitForSeconds wait = new WaitForSeconds(0.05f);
        GetComponent<MeshFilter>().mesh = mesh = new Mesh();
        mesh.name = "Procedural Grid";

        vertices = new Vector3[(xSize + 1) * (ySize + 1)];
        Vector2[] uv = new Vector2[vertices.Length];
        // generating data
        for (int i = 0, y = 0; y <= ySize; y++)
        {
            for (int x = 0; x <= xSize; x++, i++)
            {
                vertices[i] = new Vector3(x, y);
                uv[i] = new Vector2((float) x / xSize, (float) y / ySize);
            }
        }
        // pass data to gpu
        mesh.vertices = vertices;
        mesh.uv = uv;

        int[] triangles = new int[xSize * ySize * 6];
        for (int ti = 0, vi = 0, y = 0; y < ySize; y++, vi++)
        {
            for (int x = 0; x < xSize; x++, ti += 6, vi++)
            {
                triangles[ti] = vi;
                triangles[ti + 3] = triangles[ti + 2] = vi + 1;
                triangles[ti + 4] = triangles[ti + 1] = vi + xSize + 1;
                triangles[ti + 5] = vi + xSize + 2;
                mesh.triangles = triangles;
                yield return wait;
            }
        }
        mesh.RecalculateNormals();

        /*for (int i = 0, ii = 0, x = 0; x < xSize; i += 6, ii++, x++)
        {
            triangles[i] = triangles[i + 3] = ii + xSize;
            triangles[i + 1] = triangles[i + 5] = ii + 1;
            triangles[i + 2] = ii;
            triangles[i + 4] = ii + xSize + 1;
            mesh.triangles = triangles;
            yield return wait;
        }*/
    }
}

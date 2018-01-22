// read tutorial here:
// http://catlikecoding.com/unity/tutorials/procedural-grid/

using System.Collections;
using UnityEngine;

// Grid script requires the GameObject to have a MeshFilter
// and MeshRenderer component
[RequireComponent(typeof(MeshFilter), typeof(MeshRenderer))]
public class Grid : MonoBehaviour {

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

        mesh = gameObject.GetComponent<MeshFilter>().mesh = new Mesh();
        mesh.name = "Procedural Grid";

        vertices = new Vector3[(xSize + 1) * (ySize + 1)];
        for (int i = 0, y = 0; y < ySize; y++)
        {
            for (int x = 0; x < xSize; x++, i++)
            {
                vertices[i] = new Vector3(x, y);
                yield return wait;
            }
        }
        // defining the VBO
        mesh.vertices = vertices;
        
        // defining the EBO
        int[] triangles = new int[6];
        yield return wait;
        // quad indices assignation
        /*
         * 
        triangles[0] = xSize;
        triangles[1] = 1;
        triangles[2] = 0;
        triangles[3] = xSize;
        triangles[4] = xSize + 1;
        triangles[5] = 1;*/
        // abreviated version of last commented code
        triangles[0] = triangles[3] = xSize;
        triangles[1] = triangles[5] = 1;
        triangles[2] = 0;
        triangles[4] = xSize + 1;
        mesh.triangles = triangles;
    }
}

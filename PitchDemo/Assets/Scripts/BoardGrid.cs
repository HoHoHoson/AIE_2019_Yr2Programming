using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BoardGrid : MonoBehaviour
{
    public Vector2 boardDimensions;
    public GameObject tile;
    public float tileRadius;
    public bool replaceColour = true;
    public Material black;
    public Material white;

    float tileDiameter;
    float tileThickness;
    int boardSizeX, boardSizeZ;
    public BoardNode[,] nodeArray;

	// Use this for initialization
	void Awake ()
    {
        boardSizeX = (int)boardDimensions.x;
        boardSizeZ = (int)boardDimensions.y;

        if (tileRadius == 0)
            tileRadius = tile.GetComponentInChildren<MeshRenderer>().bounds.extents.x;

        tileThickness = tile.GetComponentInChildren<MeshRenderer>().bounds.extents.y * 2;
        tileDiameter = tileRadius * 2;

        createGrid();
	}

    void createGrid()
    {
        nodeArray = new BoardNode[boardSizeX, boardSizeZ];
        Vector3 worldBottomLeftTile = transform.position - Vector3.right * (boardSizeX / 2 * tileDiameter - tileRadius) 
                                                         - Vector3.forward * (boardSizeZ / 2 * tileDiameter - tileRadius)
                                                         + Vector3.up * tileThickness;

        for (int x = 0; x < boardSizeX; ++x)
            for (int z = 0; z < boardSizeZ; ++z)
            {
                Vector3 nodePos = worldBottomLeftTile + Vector3.right * (x * tileDiameter)
                                                      + Vector3.forward * (z * tileDiameter);
                nodeArray[x, z] = new BoardNode(nodePos, new Vector2(x, z));
            }

        buildBoard();
    }

    public List<BoardNode> getNeighbours(BoardNode node)
    {
        List<BoardNode> neighbours = new List<BoardNode>();

        for (int x = -1; x <= 1; ++x)
            for (int y = -1; y <= 1; ++y)
            {
                if (x == 0 && y == 0)
                    continue;

                int xPos = (int)node.boardPosition.x + x;
                int yPos = (int)node.boardPosition.y + y;

                if (xPos < 0 || yPos < 0 || xPos >= boardSizeX || yPos >= boardSizeZ)
                    continue;

                neighbours.Add(nodeArray[xPos, yPos]);
            }

        return neighbours;
    }

    private void buildBoard()
    {
        if (nodeArray != null)
        {
            bool isBlack = true;

            for (int z = 0; z < boardSizeZ; ++z)
            {
                for (int x = 0; x < boardSizeX; ++x)
                {
                    Material colour = isBlack ? black : white;
                    GameObject tileGO = Instantiate(tile);
                    tileGO.transform.position = nodeArray[x, z].worldPosition;

                    if (replaceColour)
                        tileGO.GetComponentInChildren<Renderer>().material = colour;

                    TileData td = tileGO.AddComponent<TileData>();
                    td.node = nodeArray[x, z];

                    isBlack = !isBlack;
                }

                isBlack = !isBlack;
            }
        }
    }
}

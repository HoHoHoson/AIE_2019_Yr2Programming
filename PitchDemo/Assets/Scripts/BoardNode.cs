using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BoardNode
{
    public Vector3 worldPosition;

    public int gCost;
    public int hCost;

    public BoardNode(Vector3 worldPos)
    {
        worldPosition = worldPos;
    }

    public int fCost
    {
        get { return gCost + hCost; }
    }
}

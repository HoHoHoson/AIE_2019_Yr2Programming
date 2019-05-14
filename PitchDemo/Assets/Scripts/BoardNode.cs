using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BoardNode
{
    public Vector3 worldPosition;
    public Vector2 boardPosition;

    public int gCost;
    public int hCost;

    public BoardNode parent; 
    public Unit occupiedUnit = null;

    public BoardNode(Vector3 worldPos, Vector2 boardPos)
    {
        worldPosition = worldPos;
        boardPosition = boardPos;
    }

    public int fCost
    {
        get { return gCost + hCost; }
    }
}

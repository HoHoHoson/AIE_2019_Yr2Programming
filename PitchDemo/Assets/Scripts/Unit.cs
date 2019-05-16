using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Unit : MonoBehaviour
{
    public enum Team
    {
        Green,
        Red
    }

    private BoardGrid board;
    public BoardNode occupiedNode;
    public Team team;

    public void setTeam(Team state)
    {
        team = state;

        if (team == Team.Red)
            transform.LookAt(transform.position - Vector3.forward);
    }

	// Use this for initialization
	void Awake ()
    {
        board = GameObject.FindGameObjectWithTag("Board").GetComponent<BoardGrid>();
	}

    public void updateUnit()
    {
        transform.root.position = occupiedNode.worldPosition;

        if (team == Team.Green)
            gameObject.GetComponentInChildren<Renderer>().material.color = Color.green;
        else
            gameObject.GetComponentInChildren<Renderer>().material.color = Color.red;
    }

    public void pathFind(BoardNode targetNode)
    {
        List<BoardNode> openNodes = new List<BoardNode>();
        HashSet<BoardNode> closedNodes = new HashSet<BoardNode>();

        BoardNode currentNode = occupiedNode;
        currentNode.gCost = 0;
        currentNode.hCost = getDistance(targetNode, occupiedNode);
        currentNode.parent = null;

        openNodes.Add(occupiedNode);

        while(openNodes.Count > 0)
        {
            currentNode = openNodes[0];

            foreach (BoardNode n in openNodes)
                if (n.fCost < currentNode.fCost || n.fCost == currentNode.fCost && n.hCost < currentNode.hCost)
                    currentNode = n;

            openNodes.Remove(currentNode);
            closedNodes.Add(currentNode);

            foreach (BoardNode n in board.getNeighbours(currentNode))
            {
                int gCostToNeighbour = currentNode.gCost + getDistance(n, currentNode);

                if (n == targetNode)
                {
                    moveUnit(currentNode, targetNode);
                    return;
                }

                if (n.occupiedUnit != null || closedNodes.Contains(n))
                    continue;

                if (gCostToNeighbour < currentNode.gCost || openNodes.Contains(n) == false)
                {
                    n.gCost = gCostToNeighbour;
                    n.hCost = getDistance(targetNode, n);
                    n.parent = currentNode;

                    if (openNodes.Contains(n) == false)
                        openNodes.Add(n);
                }
            }
        }
    }

    public int getDistance(BoardNode targetNode, BoardNode startNode)
    {
        int disX = (int)Mathf.Abs(startNode.boardPosition.x - targetNode.boardPosition.x);
        int disZ = (int)Mathf.Abs(startNode.boardPosition.y - targetNode.boardPosition.y);

        if (disX < disZ)
            return disX * 14 + (disZ - disX) * 10;
        else
            return disZ * 14 + (disX - disZ) * 10;
    }

    public void moveUnit(BoardNode pathHead, BoardNode target)
    {
        List<BoardNode> path = new List<BoardNode>();
        BoardNode current = pathHead;

        while (current.parent != null)
        {
            path.Add(current);
            current = current.parent;
        }

        if (path.Count <= 0)
        {
            transform.LookAt(target.worldPosition);
            return;
        }

        path.Reverse();


        occupiedNode.occupiedUnit = null;
        // Move a node closer to target
        occupiedNode = path[0];
        transform.LookAt(occupiedNode.worldPosition);
        occupiedNode.occupiedUnit = this;
    }

    public void setNode(int x, int z)
    {
        if (occupiedNode != null)
        {
            occupiedNode.occupiedUnit = null;
            occupiedNode = null;
        }

        occupiedNode = board.nodeArray[x, z];
        occupiedNode.occupiedUnit = this;
    }
}

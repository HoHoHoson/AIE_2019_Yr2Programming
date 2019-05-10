using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Unit : MonoBehaviour
{
    private BoardGrid board;
    private BoardNode occupiedNode;
    private List<Unit> allUnits = new List<Unit>();

	// Use this for initialization
	void Awake ()
    {
        board = GameObject.FindGameObjectWithTag("Board").GetComponent<BoardGrid>();
	}

    public void move()
    {

    }
}

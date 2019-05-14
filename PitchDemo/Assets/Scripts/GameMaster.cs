using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameMaster : MonoBehaviour
{
    public GameObject testUnit;
    public BoardGrid unitBoard;

    List<Unit> unitList = new List<Unit>();
    List<Unit> standbyList = new List<Unit>();
    bool isMoving = false;

	// Use this for initialization
	void Start ()
    {
        for (int i = 0; i < 3; ++i)
            addUnitToBoard();

        Unit u = Instantiate(testUnit).GetComponentInChildren<Unit>();
        u.setNode(0, 0);
        u.setTeam(Unit.Team.Green);
        unitList.Add(u);

        u = Instantiate(testUnit).GetComponentInChildren<Unit>();
        u.setNode(7, 7);
        u.setTeam(Unit.Team.Red);
        unitList.Add(u);

        u = Instantiate(testUnit).GetComponentInChildren<Unit>();
        u.setNode(0, 7);
        u.setTeam(Unit.Team.Red);
        unitList.Add(u);

        u = Instantiate(testUnit).GetComponentInChildren<Unit>();
        u.setNode(3, 6);
        u.setTeam(Unit.Team.Red);
        unitList.Add(u);
    }
	
	// Update is called once per frame
	void Update ()
    {
        if (isMoving == false)
            StartCoroutine("unitTurns");

        foreach (Unit u in standbyList)
            u.updateUnit();

        foreach (Unit u in unitList)
            u.updateUnit();

        if (Input.GetKeyDown(KeyCode.Mouse0))
        {

        }
    }

    IEnumerator unitTurns()
    {
        isMoving = true;

        foreach (Unit current in unitList)
        {
            Unit closest = null;

            foreach (Unit target in unitList)
            {
                if (current.team != target.team)
                    if (closest == null || current.getDistance(target.occupiedNode, current.occupiedNode) < current.getDistance(closest.occupiedNode, current.occupiedNode))
                    {
                        closest = target;
                    }
            }

            if (closest != null)
                current.pathFind(closest.occupiedNode);
        }

        yield return new WaitForSeconds(0.5f);

        isMoving = false;
    }

    private void addUnitToBoard()
    {
        Unit newUnit = Instantiate(testUnit).GetComponentInChildren<Unit>();

        for (int i = 0; i < 8; ++i)
        {
            BoardNode node = unitBoard.nodeArray[i, 0];
            if (node.occupiedUnit == null)
            {
                newUnit.occupiedNode = node;
                node.occupiedUnit = newUnit;
                newUnit.setTeam(Unit.Team.Green);

                standbyList.Add(newUnit);
                newUnit = null;

                break;
            }
        }

        if (newUnit != null)
            Destroy(newUnit);
    }
}

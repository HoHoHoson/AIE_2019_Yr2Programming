﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

/// <summary>
/// Class made for highlighting objects and changing their materials via UI and user input
/// </summary>
public class ObjectSelector : MonoBehaviour
{
    public Camera mainCam = null;
    public Dropdown dropdown = null;
    public ParticleSystem highlightParticle = null;
    public List<Material> materials;

    private Renderer selectedObj = null;

	// Use this for initialization
	void Start ()
    {
        dropdown.options.Clear();

        for (int i = 0; i < materials.Count; ++i)
            dropdown.options.Add(new Dropdown.OptionData() { text = materials[i].name });

        dropdown.onValueChanged.AddListener(delegate { DropdownChanged(dropdown); });
        dropdown.gameObject.SetActive(false);

        highlightParticle.Stop();
    }

    private void OnDestroy()
    {
        dropdown.onValueChanged.RemoveAllListeners();
    }

    // Update is called once per frame
    void Update ()
    {
        // "LeftClick" will select an object if the cursor is over a valid and non UI object
        // The object will be highlighted with a particle effect
        if (Input.GetKeyDown(KeyCode.Mouse0) && EventSystem.current.IsPointerOverGameObject() == false && mainCam != null)
        {
            RaycastHit hit;
            Ray ray = mainCam.ScreenPointToRay(Input.mousePosition);

            if (Physics.Raycast(ray, out hit))
            {
                if (selectedObj != null)
                    selectedObj.material = materials[dropdown.value];

                selectedObj = hit.transform.GetComponent<Renderer>();

                if (selectedObj != null)
                {
                    bool matMatch = false;   
                    string matName = selectedObj.material.name.Replace(" (Instance)", "");
                    
                    for (int i = 0; i < materials.Count; ++i)
                    {
                        if (materials[i].name == matName)
                        {
                            dropdown.gameObject.SetActive(true);
                            dropdown.value = i;

                            highlightParticle.Clear();
                            highlightParticle.Play();

                            matMatch = true;
                            break;
                        }
                    }

                    if (matMatch == false)
                    {
                        highlightParticle.Stop();
                        DisableDropdown(dropdown);
                    }
                }
            }
        }

        // Deselects the selected object with "Spacebar"
        if (selectedObj != null && Input.GetKeyDown(KeyCode.Space))
        {
            selectedObj = null;

            highlightParticle.Stop();
            DisableDropdown(dropdown);
        }

        if (selectedObj != null && highlightParticle.isPlaying)
            highlightParticle.transform.position = selectedObj.transform.position;
    }

    /// <summary>
    /// Changes the selected object's material if the dropdown box, <paramref name="dd"/>, has been changed
    /// </summary>
    /// <param name="dd"></param>
    private void DropdownChanged(Dropdown dd)
    {
        selectedObj.material = materials[dd.value];
    }

    /// <summary>
    /// Hides the dropdown box, <paramref name="dd"/>, if no object has been selected
    /// </summary>
    /// <param name="dd"></param>
    private void DisableDropdown(Dropdown dd)
    {
        Transform dropdownBox = dd.transform.Find("Dropdown List");

        if (dropdownBox != null)
            Destroy(dropdownBox.gameObject);

        dd.gameObject.SetActive(false);
    }
}

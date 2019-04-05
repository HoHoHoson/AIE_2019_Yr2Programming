using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

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

        if (selectedObj != null && Input.GetKeyDown(KeyCode.Space))
        {
            selectedObj = null;

            highlightParticle.Stop();
            DisableDropdown(dropdown);
        }

        if (selectedObj != null && highlightParticle.isPlaying)
            highlightParticle.transform.position = selectedObj.transform.position;
    }

    private void DropdownChanged(Dropdown dd)
    {
        selectedObj.material = materials[dd.value];
    }

    private void DisableDropdown(Dropdown dd)
    {
        Transform dropdownBox = dd.transform.Find("Dropdown List");

        if (dropdownBox != null)
            Destroy(dropdownBox.gameObject);

        dd.gameObject.SetActive(false);
    }
}

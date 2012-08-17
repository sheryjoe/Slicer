/*==============================================================================

  Program: 3D Slicer

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Nicole Aucoin, BWH

==============================================================================*/

// QT includes
#include <QApplication>
#include <QTimer>

// qMRML includes
#include "qMRMLNodeComboBox.h"

// MRML includes
#include <vtkMRMLNode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLScalarVolumeNode.h>

// VTK includes
#include "vtkSmartPointer.h"

// STD includes

// test the filtering with many cases
int qMRMLNodeComboBoxTest7( int argc, char * argv [] )
{
  QApplication app(argc, argv);

  vtkSmartPointer<vtkMRMLScene> scene =  vtkSmartPointer<vtkMRMLScene>::New();
  
  vtkSmartPointer<vtkMRMLScalarVolumeNode> noAttributeNode = vtkSmartPointer<vtkMRMLScalarVolumeNode>::New();
  scene->AddNode(noAttributeNode);
  
  const char *testingAttributeName = "testingAttribute";
  const char *testingAttribute = noAttributeNode->GetAttribute(testingAttributeName);
  std::cout << "Volume node with no call to SetAttribute, GetAttribute returns " << (testingAttribute ? testingAttribute : "0") << "." << std::endl;
  
  vtkSmartPointer<vtkMRMLScalarVolumeNode> emptyStringAttributeNode = vtkSmartPointer<vtkMRMLScalarVolumeNode>::New();
  emptyStringAttributeNode->SetAttribute(testingAttributeName, "");
  scene->AddNode(emptyStringAttributeNode);

  testingAttribute = emptyStringAttributeNode->GetAttribute(testingAttributeName);
  std::cout << "Volume node with SetAttribute called with an empty string, GetAttribute returns " << (testingAttribute ? testingAttribute : "0") << "." << std::endl;

  vtkSmartPointer<vtkMRMLScalarVolumeNode> validAttributeNode = vtkSmartPointer<vtkMRMLScalarVolumeNode>::New();
  validAttributeNode->SetAttribute(testingAttributeName, "a");
  scene->AddNode(validAttributeNode);

  testingAttribute = validAttributeNode->GetAttribute(testingAttributeName);
  std::cout << "Volume node with SetAttribute called with 'a', GetAttribute returns " << (testingAttribute ? testingAttribute : "0") << "." << std::endl;

  // a node selector with no filtering attribute, three volumes should be
  // counted
  qMRMLNodeComboBox nodeSelector;
  nodeSelector.setNodeTypes(QStringList("vtkMRMLScalarVolumeNode"));
  nodeSelector.setMRMLScene(scene);

  int nodeCount = nodeSelector.nodeCount();
  if (nodeCount != 3)
    {
    std::cerr << "qMRMLNodeComboBox:: no filtering on attribute string doesn't yield 3 nodes, got nodeCount = " << nodeCount << std::endl;
    return EXIT_FAILURE;
    }
  else
    {
    std::cout << "Passed with no filtering\n" << std::endl;
    }
  nodeSelector.show();
  
  // a node selector with a defined filtering attribute, only one volume
  // should be counted
  qMRMLNodeComboBox nodeSelectorA;
  nodeSelectorA.setNodeTypes(QStringList("vtkMRMLScalarVolumeNode"));
  nodeSelectorA.addAttribute("vtkMRMLScalarVolumeNode", testingAttributeName, "a");
  nodeSelectorA.setMRMLScene(scene);

  nodeCount = nodeSelectorA.nodeCount();
  if (nodeCount != 1)
    {
    std::cerr << "qMRMLNodeComboBox:: filtering on attribute string 'a' doesn't yield a single node, got nodeCount = " << nodeCount << std::endl;
    return EXIT_FAILURE;
    }
  else
    {
    std::cout << "Passed filtering on 'a'\n" << std::endl;
    }
  nodeSelectorA.show();

  // a node selector with a defined filtering attribute that doens't match any
  // volumes, count should be zero
  qMRMLNodeComboBox nodeSelectorB;
  nodeSelectorB.setNodeTypes(QStringList("vtkMRMLScalarVolumeNode"));
  nodeSelectorB.addAttribute("vtkMRMLScalarVolumeNode", testingAttributeName, "b");
  nodeSelectorB.setMRMLScene(scene);

  nodeCount = nodeSelectorB.nodeCount();
  if (nodeCount != 0)
    {
    std::cerr << "qMRMLNodeComboBox:: filtering on attribute string 'b' doesn't yield no nodes, got nodeCount = " << nodeCount << std::endl;
    return EXIT_FAILURE;
    }
  else
    {
    std::cout << "Passed filtering on 'b'\n" << std::endl;
    }
  nodeSelectorB.show();
  
  // a node selector with an empty string as the filtering attribute, only one
  // volume should be counted
  qMRMLNodeComboBox nodeSelectorEmpty;
  nodeSelectorEmpty.setNodeTypes(QStringList("vtkMRMLScalarVolumeNode"));
  nodeSelectorEmpty.addAttribute("vtkMRMLScalarVolumeNode", testingAttributeName, "");
  nodeSelectorEmpty.setMRMLScene(scene);

  nodeCount = nodeSelectorEmpty.nodeCount();
  if (nodeCount != 1)
    {
    std::cerr << "qMRMLNodeComboBox:: filtering on attribute string '' doesn't yield a single node, got nodeCount = " << nodeCount << std::endl;
    return EXIT_FAILURE;
    }
  else
    {
    std::cout << "Passed filtering on ''\n" << std::endl;
    }
  nodeSelectorEmpty.show();

  // a node selctor without a value for the filtering attribute, two volumes
  // should match (empty string and valid string)
  qMRMLNodeComboBox nodeSelectorNull;
  nodeSelectorNull.setNodeTypes(QStringList("vtkMRMLScalarVolumeNode"));
  nodeSelectorNull.addAttribute("vtkMRMLScalarVolumeNode", testingAttributeName);
  nodeSelectorNull.setMRMLScene(scene);

  nodeCount = nodeSelectorNull.nodeCount();
  if (nodeCount != 2)
    {
    std::cerr << "qMRMLNodeComboBox:: filtering on null attribute string doesn't yield two nodes, got nodeCount = " << nodeCount << std::endl;
    return EXIT_FAILURE;
    }
  else
    {
    std::cout << "Passed filtering on null\n" << std::endl;
    }
  nodeSelectorNull.show();
  
  if (argc < 2 || QString(argv[1]) != "-I")
    {
    QTimer::singleShot(200, &app, SLOT(quit()));
    }

  return app.exec();
}
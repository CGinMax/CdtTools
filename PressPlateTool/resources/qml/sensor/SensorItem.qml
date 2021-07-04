import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qaterial 1.0 as Qaterial

import PressPlateTools 1.0
import "../components"

Qaterial.Card {
    id: _root

    property var list_model: null
    signal queryVersion()
    signal configureAddr(int addr)
    signal queryState()
    signal configureState(int state)

    RowLayout {
        anchors.fill: parent
        Qaterial.Label {
            id: label_name
            text: sensor_name
            font.pixelSize: 18
            font.bold: true

            Layout.leftMargin: 5
        }

        Row {
            Qaterial.TextField {
                id: input_address
                title: qsTr("Address")
                text: address
                validator: IntValidator{ bottom: 1 }
            }
            LoadingButton {
                id: btn_configure_addr
                iconSource: "image://faicon/arrow-alt-circle-left"
                iconSize: 18
                onClickStarted: {
                    _root.configureAddr(parseInt(input_address.text))
                }
            }
        }

        Qaterial.FlatButton {
            id: btn_query_status
            text: qsTr("query")
            radius: 3
            onClicked: {
                emit: _root.queryState()
            }
        }

        Qaterial.FlatButton {
            id: btn_configure_status
            text: qsTr("configure")
            onClicked: {
                emit: _root.configureState(0x01)
            }
        }

        RowLayout {
            Chip {
                id: chip_hardware_version
                text: hardware_version

            }
            Chip {
                id: chip_software_version
                text: software_version
            }
            LoadingButton {
                id: btn_query_version
                iconSource: "image://faicon/search"
                iconSize: 18
                onClickStarted: {
                    emit: _root.queryVersion()
                }

            }
        }

        Qaterial.RoundButton {
            id: btn_delete
            icon.source: "image://faicon/trash"
            icon.width: 18
            icon.height: 18
            icon.color: "red"
            onClicked: {
                list_model.removeSensor(index)
            }
        }
    }


    function changeVersionState(success) {
        btn_query_version.changeState(success)
    }
    function changeAddress(success) {
        btn_configure_addr.changeState(success)
    }

//    function checkConnected() {
//        if (!controller.isConnected()) {
//            Qaterial.SnackbarManager.show({
//                text: "Communication not open!Can not operate!"
//            })
//            return false;
//        }
//        return true;
//    }

}

#include "ScenarioControl.hpp"
#include "Document/BaseElement/BaseElementModel.hpp"
#include "Document/BaseElement/BaseElementPresenter.hpp"
#include "Document/Constraint/ConstraintModel.hpp"
#include "Document/Event/EventModel.hpp"
#include "Document/TimeNode/TimeNodeModel.hpp"
#include "ProcessInterface/ProcessViewModelInterface.hpp"
#include "Process/ScenarioModel.hpp"
#include "Process/ScenarioGlobalCommandManager.hpp"

#include <iscore/menu/MenuInterface.hpp>
#include <iscore/plugins/documentdelegate/DocumentDelegateModelInterface.hpp>
#include <iscore/document/DocumentInterface.hpp>
#include <core/presenter/Presenter.hpp>
#include <core/presenter/MenubarManager.hpp>

#include "Control/OldFormatConversion.hpp"

#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextBlock>
#include <QJsonDocument>
#include <QGridLayout>
#include <QTextEdit>
#include <QDialogButtonBox>
class TextDialog : public QDialog
{
    public:
        TextDialog(QString s)
        {
            this->setLayout(new QGridLayout);
            auto textEdit = new QTextEdit;
            textEdit->setPlainText(s);
            layout()->addWidget(textEdit);
            auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
            layout()->addWidget(buttonBox);

            connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

        }
};

using namespace iscore;

ScenarioControl::ScenarioControl(QObject* parent) :
    PluginControlInterface {"ScenarioControl", parent},
    m_processList {new ProcessList{this}}
{

}

void ScenarioControl::populateMenus(iscore::MenubarManager* menu)
{
    auto focusedScenario = [this] () {
        auto& model = IDocument::modelDelegate<BaseElementModel>(*currentDocument());
        return dynamic_cast<ScenarioModel*>(model.focusedViewModel()->sharedProcessModel());
    };

    // File

    // Export in old format
    auto toZeroTwo = new QAction("To i-score 0.2", this);
    connect(toZeroTwo, &QAction::triggered,
            [this]()
    {
        auto savename = QFileDialog::getSaveFileName(nullptr, tr("Save"));

        if(!savename.isEmpty())
        {
            QFile f(savename);
            f.open(QIODevice::WriteOnly);
            f.write(JSONToZeroTwo(currentDocument()->saveAsJson()).toLatin1().constData());
        }
    });

    menu->insertActionIntoToplevelMenu(ToplevelMenuElement::FileMenu,
                                       FileMenuElement::Separator_Quit,
                                       toZeroTwo);


    // Edit
    QAction* removeElements = new QAction {tr("Remove scenario elements"), this};
    connect(removeElements, &QAction::triggered,
            [this,focusedScenario] ()
    {
        if(auto sm = focusedScenario())
        {
            ScenarioGlobalCommandManager mgr{currentDocument()->commandStack()};
            mgr.deleteSelection(*sm);
        }
    });
    menu->insertActionIntoToplevelMenu(ToplevelMenuElement::EditMenu,
                                       removeElements);


    QAction* clearElements = new QAction {tr("Clear scenario elements"), this};
    connect(clearElements, &QAction::triggered,
            [this,focusedScenario] ()
    {
        if(auto sm = focusedScenario())
        {
            ScenarioGlobalCommandManager mgr{currentDocument()->commandStack()};
            mgr.clearContentFromSelection(*sm);
        }
    });
    menu->insertActionIntoToplevelMenu(ToplevelMenuElement::EditMenu,
                                       clearElements);

    // View
    QAction* selectAll = new QAction {tr("Select all"), this};
    connect(selectAll,	&QAction::triggered,
            [this] ()
    {
        auto& pres = IDocument::presenterDelegate<BaseElementPresenter>(*currentDocument());
        pres.selectAll();
    });

    menu->insertActionIntoToplevelMenu(ToplevelMenuElement::ViewMenu,
                                       ViewMenuElement::Windows,
                                       selectAll);


    QAction* deselectAll = new QAction {tr("Deselect all"), this};
    connect(deselectAll,	&QAction::triggered,
            [this] ()
    {
        auto& pres = IDocument::presenterDelegate<BaseElementPresenter>(*currentDocument());
        pres.deselectAll();
    });

    menu->insertActionIntoToplevelMenu(ToplevelMenuElement::ViewMenu,
                                       ViewMenuElement::Windows,
                                       deselectAll);

    QAction* elementsToJson = new QAction {tr("Convert selection to JSON"), this};
    connect(elementsToJson,	&QAction::triggered,
            [this,focusedScenario] ()
    {
        if(auto sm = focusedScenario())
        {
            auto arrayToJson = [] (auto&& selected)
            {
                QJsonArray array;
                if(!selected.empty())
                {
                    for(auto& element : selected)
                    {
                        Visitor<Reader<JSON>> jr;
                        jr.readFrom(*element);
                        array.push_back(jr.m_obj);
                    }
                }

                return array;
            };


            QJsonObject base;
            base["Constraints"] = arrayToJson(selectedElements(sm->constraints()));
            base["Events"] = arrayToJson(selectedElements(sm->events()));
            base["TimeNodes"] = arrayToJson(selectedElements(sm->timeNodes()));


            QJsonDocument doc; doc.setObject(base);
            auto s = new TextDialog(doc.toJson(QJsonDocument::Indented));

            s->show();
        }
    });

    menu->insertActionIntoToplevelMenu(ToplevelMenuElement::ViewMenu,
                                       ViewMenuElement::Windows,
                                       elementsToJson);
}
#include "Process/Temporal/TemporalScenarioViewModel.hpp"
#include "Process/Temporal/TemporalScenarioPresenter.hpp"
#include "Process/Temporal/StateMachines/ScenarioStateMachine.hpp"
#include <QToolBar>
void ScenarioControl::populateToolbars(QToolBar* bar)
{
    // TODO make a method of this
    auto focusedScenarioStateMachine = [this] () -> ScenarioStateMachine& {
        auto& model = IDocument::modelDelegate<BaseElementModel>(*currentDocument());
        return static_cast<TemporalScenarioViewModel*>(model.focusedViewModel())->presenter()->stateMachine();
    };

    m_scenarioActionGroup = new QActionGroup(bar);
    m_scenarioActionGroup->setEnabled(false);
    auto createtool = new QAction(tr("Create"), m_scenarioActionGroup);
    createtool->setCheckable(true);
    connect(createtool, &QAction::triggered, [=] ()
    {
        emit focusedScenarioStateMachine().setCreateState();
    });

    auto movetool = new QAction(tr("Move"), m_scenarioActionGroup);
    movetool->setCheckable(true);
    connect(movetool, &QAction::triggered, [=] ()
    {
        emit focusedScenarioStateMachine().setMoveState();
    });

    auto selecttool = new QAction(tr("Select"), m_scenarioActionGroup);
    selecttool->setCheckable(true);
    selecttool->setChecked(true);
    connect(selecttool, &QAction::triggered, [=] ()
    {
        emit focusedScenarioStateMachine().setSelectState();
    });

    on_presenterChanged();
    bar->addActions(m_scenarioActionGroup->actions());
}


// Defined in CommandNames.cpp
iscore::SerializableCommand* makeCommandByName(const QString& name);

iscore::SerializableCommand* ScenarioControl::instantiateUndoCommand(const QString& name, const QByteArray& data)
{
    iscore::SerializableCommand* cmd = makeCommandByName(name);
    if(!cmd)
    {
        qDebug() << Q_FUNC_INFO << "Warning : command" << name << "received, but it could not be read.";
        return nullptr;
    }

    cmd->deserialize(data);
    return cmd;
}

void ScenarioControl::on_newDocument(iscore::Document* doc)
{
    qDebug() << Q_FUNC_INFO;
    on_presenterChanged();
}

void ScenarioControl::on_presenterChanged()
{
    // Check the current focused view model of this document
    // If it is a scenario, we enable the actiongroup, else we disable it.
    if(!m_scenarioActionGroup) return;
    if(!currentDocument())
    {
        m_scenarioActionGroup->setEnabled(false);
        return;
    }

    auto& model = IDocument::modelDelegate<BaseElementModel>(*currentDocument());

    this->disconnect(m_toolbarConnection);
    m_toolbarConnection = connect(&model,  &BaseElementModel::focusedViewModelChanged,
                                  this, [&] ()
    {
        auto onScenario = dynamic_cast<TemporalScenarioViewModel*>(model.focusedViewModel());
        m_scenarioActionGroup->setEnabled(onScenario);
    });
}

void ScenarioControl::on_documentChanged(Document* doc)
{
    on_presenterChanged();

    auto& model = IDocument::modelDelegate<BaseElementModel>(*currentDocument());
    auto onScenario = dynamic_cast<TemporalScenarioViewModel*>(model.focusedViewModel());

    m_scenarioActionGroup->setEnabled(onScenario);
}
